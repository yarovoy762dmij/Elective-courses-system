#include "entities.h"
#include "dbmanager.h"

User User::fromQuery(const QSqlQuery& q) {
    User u;
    u.id = q.value("id").toInt();
    u.login = q.value("логин").toString();
    u.email = q.value("электронная_почта").toString();
    u.roleName = q.value("название_роли").toString();
    return u;
}

QList<Elective> Elective::getAll() {
    QList<Elective> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM факультативы ORDER BY id;");
    while(q.next()) {
        Elective f;
        f.id = q.value("id").toInt();
        f.name = q.value("название_факультатива").toString();
        list.append(f);
    }
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
