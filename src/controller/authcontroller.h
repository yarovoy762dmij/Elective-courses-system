#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "../model/user.h"
#include <QString>

class AuthController {
public:
    static QString hashPassword(const QString& password);

    static bool validatePassword(const QString& password);

    static bool login(const QString& login, const QString& password, User& outUser);

    static bool registerUser(const QString& login, const QString& email, const QString& password, int roleId);
};

#endif // AUTHCONTROLLER_H
