#include "examsheetcontroller.h"
#include "model/dbmanager.h"
#include "model/examrecord.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDate>
#include <QDebug>

QList<ExamCourseRow> ExamSheetController::getAvailableCourses() {
    QList<ExamCourseRow> list;

    QString sql = "SELECT sk.id, f.название_факультатива, sk.учебный_год, sk.номер_семестра, "
                  "(SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = sk.id LIMIT 1) AS professorId "
                  "FROM семестровые_курсы sk "
                  "JOIN факультативы f ON sk.id_факультатива = f.id "
                  "ORDER BY f.название_факультатива, sk.учебный_год, sk.номер_семестра;";
    QSqlQuery q = DBManager::instance().execQuery(sql);
    while (q.next()) {
        int courseId = q.value(0).toInt();
        QString electiveName = q.value(1).toString();
        int academicYear = q.value(2).toInt();
        int semesterNumber = q.value(3).toInt();
        int professorId = q.value(4).toInt();

        list.append(ExamCourseRow(courseId, electiveName, academicYear, semesterNumber, professorId));
    }
    return list;
}

QList<ExamSheet> ExamSheetController::getSheetsForCourse(int courseId) {
    return ExamSheet::getBySemesterCourseId(courseId);
}

QList<StudentGradeRow> ExamSheetController::getStudentGrades(int sheetId) {
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
        int sId = q.value(0).toInt();
        QString sName = q.value(1).toString();
        int sGrade = q.value(2).isNull() ? 0 : q.value(2).toInt();

        list.append(StudentGradeRow(sId, sName, sGrade));
    }
    return list;
}

bool ExamSheetController::createSheet(int courseId) {
    //Преподаватель, закрепленный за данным семестровым курсом
    QString sqlFind = "SELECT id_преподавателя FROM обучение_студента WHERE id_семестрового_курса = ? LIMIT 1;";
    QSqlQuery qFind = DBManager::instance().execQuery(sqlFind, {courseId});
    int professorId = 0;
    if (qFind.next()) {
        professorId = qFind.value(0).toInt();
    }
    if (professorId == 0) {
        qDebug() << "Не найден преподаватель для курса" << courseId;
        return false;
    }

    ExamSheet sheet;
    sheet.semesterCourseId = courseId;
    sheet.professorId = professorId;
    sheet.creationDate = QDate::currentDate();

    return ExamSheet::insert(sheet);
}

bool ExamSheetController::deleteSheet(int sheetId) {
    return ExamSheet::remove(sheetId);
}

bool ExamSheetController::setGrade(int sheetId, int studentId, int grade) {
    QString sql = "INSERT INTO записи_в_ведомостях (id_ведомости, id_студента, оценка) "
                  "VALUES (?, ?, ?) "
                  "ON CONFLICT (id_ведомости, id_студента) DO UPDATE SET оценка = EXCLUDED.оценка;";
    QSqlQuery q = DBManager::instance().execQuery(sql, {sheetId, studentId, grade});
    return q.isActive();
}
