#include "studentcontroller.h"
#include "model/dbmanager.h"
#include "model/student.h"
#include "session.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

QList<Student> StudentController::getAll()
{
    QList<Student> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM студенты ORDER BY id;");
    while (q.next()) {
        list.append(Student::fromQuery(q));
    }
    return list;
}

std::optional<Student> StudentController::getById(int id)
{
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM студенты WHERE id = ?;", {id});
    if (q.next()) {
        return Student::fromQuery(q);
    }
    return std::nullopt;
}

bool StudentController::create(const Student& student)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

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

bool StudentController::update(const Student& student)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "UPDATE студенты SET фамилия = ?, имя = ?, отчество = ?, телефон = ?, адрес = ?, "
        "минимальное_количество_предметов = ? WHERE id = ?;",
        {student.lastName, student.firstName,
         student.middleName.isEmpty() ? QVariant() : student.middleName,
         student.phone,
         student.address.isEmpty() ? QVariant() : student.address,
         student.minSubjectCount, student.id});
    return q.isActive() && q.numRowsAffected() > 0;
}

bool StudentController::remove(int id)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM студенты WHERE id = ?;", {id});
    return q.isActive();
}
