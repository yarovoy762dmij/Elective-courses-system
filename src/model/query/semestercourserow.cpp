#include "semestercourserow.h"

SemesterCourseRow SemesterCourseRow::fromQuery(const QSqlQuery& q)
{
    SemesterCourseRow row;
    row.courseId = q.value("id").toInt();
    row.electiveId = q.value("id_факультатива").toInt();
    row.academicYear = q.value("учебный_год").toInt();
    row.semesterNumber = q.value("номер_семестра").toInt();
    row.lectureHours = q.value("часы_лекций").toInt();
    row.practiceHours = q.value("часы_практик").toInt();
    row.labHours = q.value("часы_лабораторных").toInt();
    row.professorName = q.value("преподаватель").toString();
    return row;
}
