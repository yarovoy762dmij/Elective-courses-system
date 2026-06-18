#ifndef EXAMCOURSEROW_H
#define EXAMCOURSEROW_H

#include <QString>

class ExamCourseRow {
public:
    ExamCourseRow();
    ExamCourseRow(int courseId, const QString &electiveName, int academicYear, int semesterNumber, int professorId);

    int courseId() const;
    QString electiveName() const;
    int academicYear() const;
    int semesterNumber() const;
    int professorId() const;

    QString getDisplayText() const;

private:
    int m_courseId;
    QString m_electiveName;
    int m_academicYear;
    int m_semesterNumber;
    int m_professorId;
};

#endif // EXAMCOURSEROW_H
