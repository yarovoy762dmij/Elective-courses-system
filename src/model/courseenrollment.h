#ifndef COURSEENROLLMENT_H
#define COURSEENROLLMENT_H

#include <QList>
#include <QSqlQuery>
#include <optional>

class CourseEnrollment {                                        //прохождение_курсов
public:
    int electiveId = 0;
    int studentId = 0;
    std::optional<int> finalGrade;

    static CourseEnrollment fromQuery(const QSqlQuery& q);
    static QList<CourseEnrollment> getAll();
    static bool insert(int electiveId, int studentId, std::optional<int> finalGrade = std::nullopt);
    static bool remove(int electiveId, int studentId);
};

#endif // COURSEENROLLMENT_H
