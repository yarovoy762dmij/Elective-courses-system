#include "examsheet.h"
#include "dbmanager.h"

ExamSheet ExamSheet::fromQuery(const QSqlQuery& q) {
    ExamSheet es;
    es.id = q.value("id").toInt();
    es.creationDate = q.value("дата_создания").toDate();
    es.semesterCourseId = q.value("id_семестрового_курса").toInt();
    es.teacherId = q.value("id_преподавателя").toInt();
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

bool ExamSheet::insert(const ExamSheet& sheet) {
    QVariantList params = {sheet.semesterCourseId, sheet.teacherId};
    QString sql;
    if (sheet.creationDate.isValid()) {
        sql = "INSERT INTO экзаменационные_ведомости "
              "(дата_создания, id_семестрового_курса, id_преподавателя) VALUES (?, ?, ?);";
        params.prepend(sheet.creationDate);
    } else {
        sql = "INSERT INTO экзаменационные_ведомости "
              "(id_семестрового_курса, id_преподавателя) VALUES (?, ?);";
    }
    QSqlQuery q = DBManager::instance().execQuery(sql, params);
    return q.isActive();
}

bool ExamSheet::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM экзаменационные_ведомости WHERE id = ?;", {id});
    return q.isActive();
}
