#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QList>
#include "model/user.h"

class UserController {
public:
    static QList<User> getAllUsers();
    static bool changeRole(int userId, int roleId);
};

#endif // USERCONTROLLER_H
