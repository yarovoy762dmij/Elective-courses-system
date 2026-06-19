#include "enrollmentcontroller.h"
#include "model/dbmanager.h"
#include "model/courseenrollment.h"
#include "studentcontroller.h"
#include "session.h"
#include <QSqlQuery>
#include <QVariant>

bool EnrollmentController::enrollStudent(int studentId, int electiveId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    auto studentOpt = StudentController::getById(studentId);
    if (!studentOpt) {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO прохождения_курсов (id_факультатива, id_студента, итоговая_оценка) VALUES (?, ?, ?);",
        {electiveId, studentId, QVariant()});
    return q.isActive();
}

bool EnrollmentController::unenrollStudent(int studentId, int electiveId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист") {
        return false;
    }

    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM прохождения_курсов WHERE id_факультатива = ? AND id_студента = ?;",
        {electiveId, studentId});
    return q.isActive();
}

QList<CourseEnrollment> EnrollmentController::getEnrollmentsByStudent(int studentId)
{
    QList<CourseEnrollment> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM прохождения_курсов WHERE id_студента = ? ORDER BY id_факультатива;",
        {studentId});
    while (q.next()) {
        list.append(CourseEnrollment::fromQuery(q));
    }
    return list;
}

int EnrollmentController::getEnrolledCount(int studentId)
{
    return getEnrollmentsByStudent(studentId).size();
}

bool EnrollmentController::isMinReached(int studentId)
{
    auto studentOpt = StudentController::getById(studentId);
    if (!studentOpt) {
        return false;
    }

    int minCount = studentOpt->minSubjectCount;
    if (minCount == 0) {
        return true;
    }

    return getEnrolledCount(studentId) >= minCount;
}
