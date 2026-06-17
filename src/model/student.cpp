#include "student.h"
#include "dbmanager.h"
#include <QVariant>
#include <QSqlError>
#include <QDebug>

Student Student::fromQuery(const QSqlQuery& q) {
    Student s;
    s.id = q.value("id").toInt();
    s.lastName = q.value("фамилия").toString();
    s.firstName = q.value("имя").toString();
    s.middleName = q.value("отчество").toString();
    s.phone = q.value("телефон").toString();
    s.address = q.value("адрес").toString();
    s.minSubjectCount = q.value("минимальное_количество_предметов").toInt();
    return s;
}

QList<Student> Student::getAll() {
    QList<Student> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM студенты ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

std::optional<Student> Student::getById(int id) {
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM студенты WHERE id = ?;", {id});
    if (q.next())
        return fromQuery(q);
    return std::nullopt;
}

bool Student::insert(const Student& student) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO студенты (фамилия, имя, отчество, телефон, адрес, минимальное_количество_предметов) "
        "VALUES (?, ?, ?, ?, ?, ?);",
        {student.lastName, student.firstName,
         student.middleName.isEmpty() ? QVariant() : student.middleName,
         student.phone,
         student.address.isEmpty() ? QVariant() : student.address,
         student.minSubjectCount});

    if (!q.isActive()) {
        qDebug() << "ОШИБКА ДОБАВЛЕНИЯ СТУДЕНТА:" << q.lastError().text();
    }
    return q.isActive();
}

bool Student::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery("DELETE FROM студенты WHERE id = ?;", {id});
    return q.isActive();
}

bool Student::update() {
    QSqlQuery q = DBManager::instance().execQuery(
        "UPDATE студенты SET фамилия = ?, имя = ?, отчество = ?, телефон = ?, адрес = ?, "
        "минимальное_количество_предметов = ? WHERE id = ?;",
        {lastName, firstName,
         middleName.isEmpty() ? QVariant() : middleName,
         phone,
         address.isEmpty() ? QVariant() : address,
         minSubjectCount, id});
    return q.isActive() && q.numRowsAffected() > 0;
}
