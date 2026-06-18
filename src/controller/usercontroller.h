#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QList>
#include <QString>
#include <QMap>

struct UserAdminRow {
    int id;
    QString login;
    QString email;
    QString roles;
};

class UserController {
public:
    static QList<UserAdminRow> getAllUsersForAdmin();

    static QMap<int, QString> getUserRoles(int userId);

    static QMap<int, QString> getAvailableRoles(int userId);

    static bool addRole(int userId, int roleId);
    static bool removeRole(int userId, int roleId);
};

#endif // USERCONTROLLER_H
