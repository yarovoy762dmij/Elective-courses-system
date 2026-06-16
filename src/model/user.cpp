#include "user.h"
#include "dbmanager.h"

User User::fromQuery(const QSqlQuery& q) {
    User u;
    u.id = q.value("id").toInt();
    u.login = q.value("логин").toString();
    u.email = q.value("электронная_почта").toString();
    u.roleName = q.value("название_роли").toString();
    return u;
}

QList<User> User::getAll() {
    QList<User> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT p.id, p.логин, p.электронная_почта, r.название_роли "
        "FROM пользователи p "
        "JOIN роли_пользователей rp ON p.id = rp.id_пользователя "
        "JOIN роли r ON rp.id_роли = r.id "
        "ORDER BY p.id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}
