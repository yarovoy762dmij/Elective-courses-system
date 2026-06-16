#include "student.h"
#include "dbmanager.h"
#include <QVariant>

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

bool Student::insert(const Student& student) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO студенты (фамилия, имя, отчество, телефон, адрес, минимальное_количество_предметов) "
        "VALUES (?, ?, ?, ?, ?, ?);",
        {student.lastName, student.firstName,
         student.middleName.isEmpty() ? QVariant() : student.middleName,
         student.phone,
         student.address.isEmpty() ? QVariant() : student.address,
         student.minSubjectCount});
    return q.isActive();
}

bool Student::remove(int id) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM студенты WHERE id = ?;", {id});
    return q.isActive();
}
