#ifndef SEMESTERCOURSEROW_H
#define SEMESTERCOURSEROW_H

#include <QList>
#include <QSqlQuery>
#include <QString>

class SemesterCourseRow {
public:
    int courseId = 0;          //id
    int electiveId = 0;        //id_факультатива
    int academicYear = 2024;   //учебный_год
    int semesterNumber = 1;    //номер_семестра
    int lectureHours = 0;      //часы_лекций
    int practiceHours = 0;     //часы_практик
    int labHours = 0;          //часы_лабораторных
    QString professorName;     //ФИО преподавателя

    static SemesterCourseRow fromQuery(const QSqlQuery& q);
    static QList<SemesterCourseRow> getCoursesForElective(int electiveId);

    static bool insertCourse(int electiveId, int academicYear, int semester, int lec, int prac, int lab, int professorId);
    static bool updateCourse(int courseId, int academicYear, int semester, int lec, int prac, int lab, int professorId);
    static bool removeCourse(int courseId);

    static int getProfessorIdForCourse(int courseId);
};

#endif // SEMESTERCOURSEROW_H
