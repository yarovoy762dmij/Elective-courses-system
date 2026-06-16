#include "profilecontroller.h"
#include "../model/dbmanager.h"
#include "authcontroller.h"

bool ProfileController::updateBasicInfo(int userId, const QString& newLogin, const QString& newEmail)
{
    QString sql = "UPDATE пользователи SET логин = ?, электронная_почта = ? WHERE id = ?;";
    QSqlQuery q = DBManager::instance().execQuery(sql, {newLogin, newEmail, userId});

    return q.isActive();
}

bool ProfileController::changePassword(int userId, const QString& oldPassword, const QString& newPassword, QString& errorMsg)
{
    if (!AuthController::validatePassword(newPassword)) {                       //Также проверяется регулярным выражением
        errorMsg = "Новый пароль слишком простой!\n"
                   "Он должен быть не менее 8 символов, содержать цифру, "
                   "заглавную букву и специальный символ.";
        return false;
    }

    QString oldHash = AuthController::hashPassword(oldPassword);                //Проверка старого пароля
    QString checkSql = "SELECT id FROM пользователи WHERE id = ? AND хэш_пароля = ?;";
    QSqlQuery qCheck = DBManager::instance().execQuery(checkSql, {userId, oldHash});

    if (!qCheck.next()) {
        errorMsg = "Текущий (старый) пароль введен неверно!";
        return false;
    }

    QString newHash = AuthController::hashPassword(newPassword);
    QString updateSql = "UPDATE пользователи SET хэш_пароля = ? WHERE id = ?;";
    DBManager::instance().execQuery(updateSql, {newHash, userId});

    return true;
}
