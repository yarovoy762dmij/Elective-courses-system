#ifndef SEMESTERCOURSE_H
#define SEMESTERCOURSE_H

#include <QList>
#include <QSqlQuery>

class SemesterCourse {                                      //семестровые_курсы
public:
    int id = 0;
    int academicYear = 0;
    int semesterNumber = 0;
    int electiveId = 0;

    static SemesterCourse fromQuery(const QSqlQuery& q);
    static QList<SemesterCourse> getAll();
    static bool insert(const SemesterCourse& course);
    static bool remove(int id);
};

#endif // SEMESTERCOURSE_H
