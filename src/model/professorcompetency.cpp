#include "professorcompetency.h"
#include "dbmanager.h"

ProfessorCompetency ProfessorCompetency::fromQuery(const QSqlQuery& q) {
    ProfessorCompetency tc;
    tc.electiveId = q.value("id_факультатива").toInt();
    tc.teacherId = q.value("id_преподавателя").toInt();
    return tc;
}

QList<ProfessorCompetency> ProfessorCompetency::getAll() {
    QList<ProfessorCompetency> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM компетенции_преподавателей "
        "ORDER BY id_факультатива, id_преподавателя;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool ProfessorCompetency::insert(int electiveId, int teacherId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO компетенции_преподавателей (id_факультатива, id_преподавателя) "
        "VALUES (?, ?);",
        {electiveId, teacherId});
    return q.isActive();
}

bool ProfessorCompetency::remove(int electiveId, int teacherId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM компетенции_преподавателей "
        "WHERE id_факультатива = ? AND id_преподавателя = ?;",
        {electiveId, teacherId});
    return q.isActive();
}
