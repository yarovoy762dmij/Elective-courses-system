#include "elective.h"
#include "dbmanager.h"

Elective Elective::fromQuery(const QSqlQuery& q) {
    Elective e;
    e.id = q.value("id").toInt();
    e.name = q.value("название_факультатива").toString();
    return e;
}

QList<Elective> Elective::getAll() {
    QList<Elective> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM факультативы ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool Elective::insert(const QString& name) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO факультативы (название_факультатива) VALUES (?);", {name});
    return q.isActive();
}

bool Elective::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM факультативы WHERE id = ?;", {id});
    return q.isActive();
}
