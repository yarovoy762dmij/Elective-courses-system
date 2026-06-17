#ifndef ENROLLMENTCONTROLLER_H
#define ENROLLMENTCONTROLLER_H

#include <QList>
#include "model/courseenrollment.h"

class EnrollmentController {
public:
    static bool enrollStudent(int studentId, int electiveId);
    static bool unenrollStudent(int studentId, int electiveId);
    static QList<CourseEnrollment> getEnrollmentsByStudent(int studentId);
    static int getEnrolledCount(int studentId);
    static bool isMinReached(int studentId);
};

#endif // ENROLLMENTCONTROLLER_H
