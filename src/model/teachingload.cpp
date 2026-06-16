#include "teachingload.h"
#include "dbmanager.h"

TeachingLoad TeachingLoad::fromQuery(const QSqlQuery& q) {
    TeachingLoad tl;
    tl.id = q.value("id").toInt();
    tl.hoursCount = q.value("количество_часов").toInt();
    tl.lessonTypeId = q.value("id_вида_занятия").toInt();
    tl.semesterCourseId = q.value("id_семестрового_курса").toInt();
    tl.teacherId = q.value("id_преподавателя").toInt();
    return tl;
}

QList<TeachingLoad> TeachingLoad::getAll() {
    QList<TeachingLoad> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM обучение_студента ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool TeachingLoad::insert(const TeachingLoad& load) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO обучение_студента (количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя) "
        "VALUES (?, ?, ?, ?);",
        {load.hoursCount, load.lessonTypeId, load.semesterCourseId, load.teacherId});
    return q.isActive();
}

bool TeachingLoad::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM обучение_студента WHERE id = ?;", {id});
    return q.isActive();
}
