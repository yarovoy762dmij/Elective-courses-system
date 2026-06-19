#include "courseenrollment.h"

CourseEnrollment CourseEnrollment::fromQuery(const QSqlQuery& q)
{
    CourseEnrollment ce;
    ce.electiveId = q.value("id_факультатива").toInt();
    ce.studentId = q.value("id_студента").toInt();
    if (q.value("итоговая_оценка").isNull()) {
        ce.finalGrade = std::nullopt;
    } else {
        ce.finalGrade = q.value("итоговая_оценка").toInt();
    }
    return ce;
}
