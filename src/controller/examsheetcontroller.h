#ifndef EXAMSHEETCONTROLLER_H
#define EXAMSHEETCONTROLLER_H

#include <QList>
#include "model/query/examcourserow.h"
#include "model/query/studentgraderow.h"
#include "model/examsheet.h"

class ExamSheetController {
public:
    static QList<ExamCourseRow> getAvailableCourses();
    static QList<ExamSheet> getSheetsForCourse(int courseId);

    static QList<StudentGradeRow> getStudentGrades(int sheetId);

    static bool createSheet(int courseId);
    static bool deleteSheet(int sheetId);
    static bool setGrade(int sheetId, int studentId, int grade);
};

#endif // EXAMSHEETCONTROLLER_H
