#ifndef EXAMSHEET_H
#define EXAMSHEET_H

#include <QList>
#include <QSqlQuery>
#include <QDate>

class ExamSheet {                                   //экзаменационные_ведомости
public:
    int id = 0;
    QDate creationDate;
    int semesterCourseId = 0;
    int teacherId = 0;

    static ExamSheet fromQuery(const QSqlQuery& q);
    static QList<ExamSheet> getAll();
    static bool insert(const ExamSheet& sheet);
    static bool remove(int id);
};

#endif // EXAMSHEET_H
