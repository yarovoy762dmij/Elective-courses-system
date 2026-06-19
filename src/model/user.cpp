#include "user.h"

User User::fromQuery(const QSqlQuery& q)
{
    User u;
    u.id = q.value("id").toInt();
    u.login = q.value("логин").toString();
    u.email = q.value("электронная_почта").toString();
    u.roleName = q.value("название_роли").toString();
    return u;
}
