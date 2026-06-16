#include "authcontroller.h"
#include "../model/dbmanager.h"
#include <QCryptographicHash>
#include <QRegularExpression>

QString AuthController::hashPassword(const QString& password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool AuthController::validatePassword(const QString& password) {
    QRegularExpression regex("^(?=.*[0-9])(?=.*[A-Z])(?=.*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?]).{8,}$");
    return regex.match(password).hasMatch();
}

bool AuthController::login(const QString& loginStr, const QString& password, User& outUser) {
    QString hashedPwd = hashPassword(password);

    QString sql = "SELECT p.id, p.логин, p.электронная_почта, r.название_роли "
                  "FROM пользователи p "
                  "JOIN роли_пользователей rp ON p.id = rp.id_пользователя "
                  "JOIN роли r ON rp.id_роли = r.id "
                  "WHERE p.логин = ? AND p.хэш_пароля = ?;";

    QSqlQuery q = DBManager::instance().execQuery(sql, {loginStr, hashedPwd});

    if (q.next()) {
        outUser = User::fromQuery(q);
        return true;
    }
    return false;
}

bool AuthController::registerUser(const QString& loginStr, const QString& email, const QString& password, int roleId) {
    if (!validatePassword(password)) return false;

    QString hashedPwd = hashPassword(password);

    //Вставка пользователя
    QSqlQuery q1 = DBManager::instance().execQuery(
        "INSERT INTO пользователи (логин, хэш_пароля, электронная_почта) VALUES (?, ?, ?) RETURNING id;",
        {loginStr, hashedPwd, email}
        );

    if (q1.next()) {
        int newUserId = q1.value(0).toInt();
        QSqlQuery q2 = DBManager::instance().execQuery(
            "INSERT INTO роли_пользователей (id_пользователя, id_роли) VALUES (?, ?);",
            {newUserId, roleId}
            );
        return q2.isActive();
    }
    return false;
}
