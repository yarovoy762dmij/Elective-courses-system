#include "professor.h"
#include "dbmanager.h"
#include <QVariant>

Professor Professor::fromQuery(const QSqlQuery& q) {
    Professor t;
    t.id = q.value("id").toInt();
    t.lastName = q.value("фамилия").toString();
    t.firstName = q.value("имя").toString();
    t.middleName = q.value("отчество").toString();
    t.phone = q.value("телефон").toString();
    t.position = q.value("должность").toString();
    t.userId = q.value("id_пользователя").toInt();
    return t;
}

QList<Professor> Professor::getAll() {
    QList<Professor> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM преподаватели ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool Professor::insert(const Professor& professor) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO преподаватели (фамилия, имя, отчество, телефон, должность, id_пользователя) "
        "VALUES (?, ?, ?, ?, ?, ?);",
        {professor.lastName, professor.firstName,
         professor.middleName.isEmpty() ? QVariant() : professor.middleName,
         professor.phone,
         professor.position.isEmpty() ? QVariant() : professor.position,
         professor.userId});
    return q.isActive();
}

bool Professor::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM преподаватели WHERE id = ?;", {id});
    return q.isActive();
}
