#ifndef SEMESTERCOURSECONTROLLER_H
#define SEMESTERCOURSECONTROLLER_H

#include <QList>
#include "model/query/semestercourserow.h"

class SemesterCourseController {
public:
    static QList<SemesterCourseRow> getCoursesForElective(int electiveId);
    static bool createCourse(int electiveId, int academicYear, int semester, int lec, int prac, int lab, int professorId);
    static bool updateCourse(int courseId, int academicYear, int semester, int lec, int prac, int lab, int professorId);
    static bool removeCourse(int courseId);
    static int getProfessorIdForCourse(int courseId);
};

#endif // SEMESTERCOURSECONTROLLER_H
