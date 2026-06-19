#ifndef EXAMSHEET_H
#define EXAMSHEET_H

#include <QDate>
#include <QSqlQuery>

class ExamSheet {
public:
    int id = 0;
    QDate creationDate;
    int semesterCourseId = 0;
    int professorId = 0;

    static ExamSheet fromQuery(const QSqlQuery& q);
};

#endif // EXAMSHEET_H
