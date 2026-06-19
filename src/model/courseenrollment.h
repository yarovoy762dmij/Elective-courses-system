#ifndef COURSEENROLLMENT_H
#define COURSEENROLLMENT_H

#include <QSqlQuery>
#include <optional>

class CourseEnrollment {
public:
    int electiveId = 0;
    int studentId = 0;
    std::optional<int> finalGrade;

    static CourseEnrollment fromQuery(const QSqlQuery& q);
};

#endif // COURSEENROLLMENT_H
