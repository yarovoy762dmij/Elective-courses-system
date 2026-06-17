#include "enrollmentcontroller.h"
#include "model/student.h"
#include "model/courseenrollment.h"
#include "session.h"

bool EnrollmentController::enrollStudent(int studentId, int electiveId)
{
    QString role = Session::instance().currentUser().roleName;                  //Проверка прав
    if (role != "Суперпользователь" && role != "Методист")
        return false;

    //Получение студента и проверка минимального количества предметов
    auto studentOpt = Student::getById(studentId);
    if (!studentOpt)
        return false;

    return CourseEnrollment::insert(electiveId, studentId);
}

bool EnrollmentController::unenrollStudent(int studentId, int electiveId)
{
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;

    return CourseEnrollment::remove(electiveId, studentId);
}

QList<CourseEnrollment> EnrollmentController::getEnrollmentsByStudent(int studentId)
{
    //Получение всех записей студента
    return CourseEnrollment::getByStudentId(studentId);
}

int EnrollmentController::getEnrolledCount(int studentId)
{
    //Количество записанных курсов у студента
    return getEnrollmentsByStudent(studentId).size();
}

bool EnrollmentController::isMinReached(int studentId)
{
    auto studentOpt = Student::getById(studentId);
    if (!studentOpt)
        return false;

    int minCount = studentOpt->minSubjectCount;
    if (minCount == 0)
        return true;

    return getEnrolledCount(studentId) >= minCount;
}
