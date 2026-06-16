#include "elective.h"
#include "dbmanager.h"
#include <QSqlError>
#include <QDebug>

Elective Elective::fromQuery(const QSqlQuery& q) {
    Elective e;
    e.id = q.value("id").toInt();
    e.name = q.value("название_факультатива").toString();
    return e;
}

QList<Elective> Elective::getAll() {
    QList<Elective> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM факультативы ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

std::optional<Elective> Elective::getById(int id) {
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM факультативы WHERE id = ?;", {id});
    if (q.next())
        return fromQuery(q);
    return std::nullopt;
}

bool Elective::insert(const QString& name) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO факультативы (название_факультатива) VALUES (?);", {name});

    // ВЫВОДИТ ОШИБКУ В КОНСОЛЬ QT CREATOR:
    if (!q.isActive()) {
        qDebug() << "ОШИБКА ДОБАВЛЕНИЯ ФАКУЛЬТАТИВА:" << q.lastError().text();
    }
    return q.isActive();
}

bool Elective::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM факультативы WHERE id = ?;", {id});
    return q.isActive();
}

bool Elective::update() {
    QSqlQuery q = DBManager::instance().execQuery(
        "UPDATE факультативы SET название_факультатива = ? WHERE id = ?;",
        {name, id});
    return q.isActive() && q.numRowsAffected() > 0;
}
