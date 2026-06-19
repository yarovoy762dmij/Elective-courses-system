#include "examsheetcontroller.h"
#include "model/dbmanager.h"
#include "model/examsheet.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDate>
#include <QDebug>
#include <QSqlError>

QList<ExamCourseRow> ExamSheetController::getAvailableCourses()
{
    QList<ExamCourseRow> list;

    QString sql = "SELECT sk.id, f.название_факультатива, sk.учебный_год, sk.номер_семестра, "
                  "(SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = sk.id LIMIT 1) AS professorId "
                  "FROM семестровые_курсы sk "
                  "JOIN факультативы f ON sk.id_факультатива = f.id "
                  "ORDER BY f.название_факультатива, sk.учебный_год, sk.номер_семестра;";

    QSqlQuery q = DBManager::instance().execQuery(sql);
    while (q.next()) {
        list.append(ExamCourseRow(
            q.value(0).toInt(),
            q.value(1).toString(),
            q.value(2).toInt(),
            q.value(3).toInt(),
            q.value(4).toInt()));
    }
    return list;
}

QList<ExamSheet> ExamSheetController::getSheetsForCourse(int courseId)
{
    QList<ExamSheet> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM экзаменационные_ведомости WHERE id_семестрового_курса = ? ORDER BY id;",
        {courseId});
    while (q.next()) {
        list.append(ExamSheet::fromQuery(q));
    }
    return list;
}

QList<StudentGradeRow> ExamSheetController::getStudentGrades(int sheetId)
{
    QList<StudentGradeRow> list;

    QString sql = "SELECT s.id, s.фамилия || ' ' || s.имя || COALESCE(' ' || s.отчество, '') AS фио, r.оценка "
                  "FROM студенты s "
                  "JOIN прохождения_курсов pk ON s.id = pk.id_студента "
                  "JOIN семестровые_курсы sk ON pk.id_факультатива = sk.id_факультатива "
                  "JOIN экзаменационные_ведомости ev ON ev.id_семестрового_курса = sk.id "
                  "LEFT JOIN записи_в_ведомостях r ON r.id_студента = s.id AND r.id_ведомости = ev.id "
                  "WHERE ev.id = ? "
                  "ORDER BY s.фамилия, s.имя;";

    QSqlQuery q = DBManager::instance().execQuery(sql, {sheetId});
    while (q.next()) {
        int grade = q.value(2).isNull() ? 0 : q.value(2).toInt();
        list.append(StudentGradeRow(q.value(0).toInt(), q.value(1).toString(), grade));
    }
    return list;
}

bool ExamSheetController::createSheet(int courseId)
{
    QSqlQuery qFind = DBManager::instance().execQuery(
        "SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = ? LIMIT 1;",
        {courseId});

    int professorId = 0;
    if (qFind.next()) {
        professorId = qFind.value(0).toInt();
    }
    if (professorId == 0) {
        qDebug() << "Не найден преподаватель для курса" << courseId;
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO экзаменационные_ведомости (дата_создания, id_семестрового_курса, id_преподавателя) "
        "VALUES (?, ?, ?);",
        {QDate::currentDate(), courseId, professorId});
    return q.isActive();
}

bool ExamSheetController::deleteSheet(int sheetId)
{
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM экзаменационные_ведомости WHERE id = ?;",
        {sheetId});
    return q.isActive();
}

bool ExamSheetController::setGrade(int sheetId, int studentId, int grade)
{
    //1. Удаление старой записи (если есть)
    QSqlQuery qDel = DBManager::instance().execQuery(
        "DELETE FROM записи_в_ведомостях WHERE id_ведомости = ? AND id_студента = ?;",
        {sheetId, studentId});
    if (!qDel.isActive()) {
        qDebug() << "Ошибка удаления старой записи:" << qDel.lastError().text();
        return false;
    }

    //2. Вставлка новой записи
    QSqlQuery qIns = DBManager::instance().execQuery(
        "INSERT INTO записи_в_ведомостях (id_ведомости, id_студента, оценка) "
        "VALUES (?, ?, ?);",
        {sheetId, studentId, grade});
    if (!qIns.isActive()) {
        qDebug() << "Ошибка вставки новой записи:" << qIns.lastError().text();
        return false;
    }

    //3. Обновленеи итоговой оценки
    QSqlQuery qElective = DBManager::instance().execQuery(
        "SELECT sk.id_факультатива "
        "FROM экзаменационные_ведомости ev "
        "JOIN семестровые_курсы sk ON ev.id_семестрового_курса = sk.id "
        "WHERE ev.id = ?;",
        {sheetId});
    if (qElective.next()) {
        int electiveId = qElective.value(0).toInt();
        QSqlQuery qUpdate = DBManager::instance().execQuery(
            "UPDATE прохождения_курсов "
            "SET итоговая_оценка = ? "
            "WHERE id_факультатива = ? AND id_студента = ?;",
            {grade, electiveId, studentId});
        if (!qUpdate.isActive()) {
            qDebug() << "Ошибка обновления итоговой оценки:" << qUpdate.lastError().text();
        }
    } else {
        qDebug() << "Не удалось найти факультатив для ведомости" << sheetId;
    }

    return true;
}
