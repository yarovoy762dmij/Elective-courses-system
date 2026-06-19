#include "electivecontroller.h"
#include "model/dbmanager.h"
#include "model/elective.h"
#include "session.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

QList<Elective> ElectiveController::getAll()
{
    QList<Elective> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM факультативы ORDER BY id;");
    while (q.next()) {
        list.append(Elective::fromQuery(q));
    }
    return list;
}

bool ElectiveController::create(const QString& name)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO факультативы (название_факультатива) VALUES (?);", {name});

    if (!q.isActive()) {
        qDebug() << "ОШИБКА ДОБАВЛЕНИЯ ФАКУЛЬТАТИВА:" << q.lastError().text();
    }
    return q.isActive();
}

bool ElectiveController::update(int id, const QString& name)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "UPDATE факультативы SET название_факультатива = ? WHERE id = ?;",
        {name, id});
    return q.isActive() && q.numRowsAffected() > 0;
}

bool ElectiveController::remove(int id)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM факультативы WHERE id = ?;", {id});
    return q.isActive();
}
