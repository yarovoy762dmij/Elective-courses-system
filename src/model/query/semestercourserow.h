#ifndef SEMESTERCOURSEROW_H
#define SEMESTERCOURSEROW_H

#include <QSqlQuery>
#include <QString>

class SemesterCourseRow {
public:
    int courseId = 0;
    int electiveId = 0;
    int academicYear = 2024;
    int semesterNumber = 1;
    int lectureHours = 0;
    int practiceHours = 0;
    int labHours = 0;
    QString professorName;

    static SemesterCourseRow fromQuery(const QSqlQuery& q);
};

#endif // SEMESTERCOURSEROW_H
