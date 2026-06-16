#include "role.h"
#include "dbmanager.h"

Role Role::fromQuery(const QSqlQuery& q) {
    Role r;
    r.id = q.value("id").toInt();
    r.name = q.value("название_роли").toString();
    return r;
}

QList<Role> Role::getAll() {
    QList<Role> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM роли ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}
