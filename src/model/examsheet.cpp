#include "examsheet.h"
#include "dbmanager.h"

ExamSheet ExamSheet::fromQuery(const QSqlQuery& q) {
    ExamSheet es;
    es.id = q.value("id").toInt();
    es.creationDate = q.value("дата_создания").toDate();
    es.semesterCourseId = q.value("id_семестрового_курса").toInt();
    es.professorId = q.value("id_преподавателя").toInt();
    return es;
}

QList<ExamSheet> ExamSheet::getAll() {
    QList<ExamSheet> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM экзаменационные_ведомости ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

std::optional<ExamSheet> ExamSheet::getById(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM экзаменационные_ведомости WHERE id = ?;", {id});
    if (q.next())
        return fromQuery(q);
    return std::nullopt;
}

QList<ExamSheet> ExamSheet::getBySemesterCourseId(int courseId) {
    QList<ExamSheet> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM экзаменационные_ведомости WHERE id_семестрового_курса = ? ORDER BY id;", {courseId});
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

QList<ExamSheet> ExamSheet::getByProfessorId(int professorId) {
    QList<ExamSheet> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM экзаменационные_ведомости WHERE id_преподавателя = ? ORDER BY id;", {professorId});
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool ExamSheet::insert(const ExamSheet& sheet) {
    QVariantList params = {sheet.semesterCourseId, sheet.professorId};
    QString sql;
    if (sheet.creationDate.isValid()) {
        sql = "INSERT INTO экзаменационные_ведомости (дата_создания, id_семестрового_курса, id_преподавателя) VALUES (?, ?, ?);";
        params.prepend(sheet.creationDate);
    } else {
        sql = "INSERT INTO экзаменационные_ведомости (id_семестрового_курса, id_преподавателя) VALUES (?, ?);";
    }
    QSqlQuery q = DBManager::instance().execQuery(sql, params);
    return q.isActive();
}

bool ExamSheet::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM экзаменационные_ведомости WHERE id = ?;", {id});
    return q.isActive();
}

bool ExamSheet::update() {
    QSqlQuery q = DBManager::instance().execQuery(
        "UPDATE экзаменационные_ведомости SET дата_создания = ?, id_семестрового_курса = ?, id_преподавателя = ? WHERE id = ?;",
        {creationDate, semesterCourseId, professorId, id});
    return q.isActive() && q.numRowsAffected() > 0;
}
