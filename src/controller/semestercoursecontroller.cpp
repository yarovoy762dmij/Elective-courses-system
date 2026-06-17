#include "semestercoursecontroller.h"
#include "session.h"

QList<SemesterCourseRow> SemesterCourseController::getCoursesForElective(int electiveId) {
    return SemesterCourseRow::getCoursesForElective(electiveId);
}

bool SemesterCourseController::createCourse(int electiveId, int academicYear, int semester, int lec, int prac, int lab, int professorId) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return SemesterCourseRow::insertCourse(electiveId, academicYear, semester, lec, prac, lab, professorId);
}

bool SemesterCourseController::updateCourse(int courseId, int academicYear, int semester, int lec, int prac, int lab, int professorId) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return SemesterCourseRow::updateCourse(courseId, academicYear, semester, lec, prac, lab, professorId);
}

bool SemesterCourseController::removeCourse(int courseId) {
    QString role = Session::instance().currentUser().roleName;
    if (role != "Суперпользователь" && role != "Методист")
        return false;
    return SemesterCourseRow::removeCourse(courseId);
}

int SemesterCourseController::getProfessorIdForCourse(int courseId) {
    return SemesterCourseRow::getProfessorIdForCourse(courseId);
}
