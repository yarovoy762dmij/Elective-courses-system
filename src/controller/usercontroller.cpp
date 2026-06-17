#include "usercontroller.h"
#include "session.h"
#include "model/dbmanager.h"
#include <QSqlQuery>

QList<User> UserController::getAllUsers() {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь")
        return {};
    return User::getAll();
}

bool UserController::changeRole(int userId, int roleId) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь")
        return false;
    // Удаляем старые роли пользователя, вставляем новую
    QSqlQuery del = DBManager::instance().execQuery("DELETE FROM роли_пользователей WHERE id_пользователя = ?;", {userId});
    if (!del.isActive()) return false;
    QSqlQuery ins = DBManager::instance().execQuery("INSERT INTO роли_пользователей (id_пользователя, id_роли) VALUES (?, ?);", {userId, roleId});
    return ins.isActive();
}
