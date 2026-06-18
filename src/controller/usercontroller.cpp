#include "usercontroller.h"
#include "model/dbmanager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

QList<UserAdminRow> UserController::getAllUsersForAdmin() {
    QList<UserAdminRow> list;

    //Все роли пользователя в одну строку
    QString sql = "SELECT u.id, u.логин, u.электронная_почта, "
                  "COALESCE(STRING_AGG(r.название_роли, ', '), 'Нет ролей') AS роли "
                  "FROM пользователи u "
                  "LEFT JOIN роли_пользователей rp ON u.id = rp.id_пользователя "
                  "LEFT JOIN роли r ON rp.id_роли = r.id "
                  "GROUP BY u.id, u.логин, u.электронная_почта "
                  "ORDER BY u.id;";

    QSqlQuery q = DBManager::instance().execQuery(sql);
    while (q.next()) {
        UserAdminRow row;
        row.id = q.value(0).toInt();
        row.login = q.value(1).toString();
        row.email = q.value(2).toString();
        row.roles = q.value(3).toString();
        list.append(row);
    }
    return list;
}

QMap<int, QString> UserController::getUserRoles(int userId) {
    QMap<int, QString> roles;
    QString sql = "SELECT r.id, r.название_роли FROM роли r "
                  "JOIN роли_пользователей rp ON r.id = rp.id_роли "
                  "WHERE rp.id_пользователя = ?;";

    QSqlQuery q = DBManager::instance().execQuery(sql, {userId});
    while (q.next()) {
        roles[q.value(0).toInt()] = q.value(1).toString();
    }
    return roles;
}

QMap<int, QString> UserController::getAvailableRoles(int userId) {
    QMap<int, QString> roles;
    QString sql = "SELECT id, название_роли FROM роли "
                  "WHERE id NOT IN ("
                  "  SELECT id_роли FROM роли_пользователей WHERE id_пользователя = ?"
                  ");";

    QSqlQuery q = DBManager::instance().execQuery(sql, {userId});
    while (q.next()) {
        roles[q.value(0).toInt()] = q.value(1).toString();
    }
    return roles;
}

bool UserController::addRole(int userId, int roleId) {
    QString sql = "INSERT INTO роли_пользователей (id_пользователя, id_роли) VALUES (?, ?);";
    QSqlQuery q = DBManager::instance().execQuery(sql, {userId, roleId});
    return q.isActive();
}

bool UserController::removeRole(int userId, int roleId) {
    QString sql = "DELETE FROM роли_пользователей WHERE id_пользователя = ? AND id_роли = ?;";
    QSqlQuery q = DBManager::instance().execQuery(sql, {userId, roleId});
    return q.isActive();
}
