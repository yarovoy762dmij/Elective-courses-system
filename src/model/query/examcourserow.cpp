#include "examcourserow.h"

ExamCourseRow::ExamCourseRow()
    : m_courseId(0)
    , m_electiveName("")
    , m_academicYear(0)
    , m_semesterNumber(0)
    , m_professorId(0)
{}

ExamCourseRow::ExamCourseRow(int courseId, const QString &electiveName, int academicYear, int semesterNumber, int professorId)
    : m_courseId(courseId)
    , m_electiveName(electiveName)
    , m_academicYear(academicYear)
    , m_semesterNumber(semesterNumber)
    , m_professorId(professorId)
{}

int ExamCourseRow::courseId() const { return m_courseId; }
QString ExamCourseRow::electiveName() const { return m_electiveName; }
int ExamCourseRow::academicYear() const { return m_academicYear; }
int ExamCourseRow::semesterNumber() const { return m_semesterNumber; }
int ExamCourseRow::professorId() const { return m_professorId; }

QString ExamCourseRow::getDisplayText() const {
    return QString("%1 — %2 год, семестр %3")
        .arg(m_electiveName)
        .arg(m_academicYear)
        .arg(m_semesterNumber);
}
