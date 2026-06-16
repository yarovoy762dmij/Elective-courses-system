#include "semestercourse.h"
#include "dbmanager.h"

SemesterCourse SemesterCourse::fromQuery(const QSqlQuery& q) {
    SemesterCourse c;
    c.id = q.value("id").toInt();
    c.academicYear = q.value("учебный_год").toInt();
    c.semesterNumber = q.value("номер_семестра").toInt();
    c.electiveId = q.value("id_факультатива").toInt();
    return c;
}

QList<SemesterCourse> SemesterCourse::getAll() {
    QList<SemesterCourse> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM семестровые_курсы ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool SemesterCourse::insert(const SemesterCourse& course) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO семестровые_курсы (учебный_год, номер_семестра, id_факультатива) "
        "VALUES (?, ?, ?);",
        {course.academicYear, course.semesterNumber, course.electiveId});
    return q.isActive();
}

bool SemesterCourse::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM семестровые_курсы WHERE id = ?;", {id});
    return q.isActive();
}
