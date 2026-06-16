#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include <QString>

class ProfileController {
public:
    //Обновление логина и email
    static bool updateBasicInfo(int userId, const QString& newLogin, const QString& newEmail);

    //Смена пароля с проверкой старого и с валидацией нового
    static bool changePassword(int userId, const QString& oldPassword, const QString& newPassword, QString& errorMsg);
};

#endif // PROFILECONTROLLER_H
