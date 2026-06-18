#include "model/query/studentgraderow.h"

StudentGradeRow::StudentGradeRow()
    : m_studentId(0)
    , m_studentFullName("")
    , m_grade(0)
{}

StudentGradeRow::StudentGradeRow(int studentId, const QString &studentFullName, int grade)
    : m_studentId(studentId)
    , m_studentFullName(studentFullName)
    , m_grade(grade)
{}

int StudentGradeRow::studentId() const {
    return m_studentId;
}

QString StudentGradeRow::studentFullName() const {
    return m_studentFullName;
}

int StudentGradeRow::grade() const {
    return m_grade;
}

void StudentGradeRow::setStudentId(int id) {
    m_studentId = id;
}

void StudentGradeRow::setStudentFullName(const QString &fullName) {
    m_studentFullName = fullName;
}

void StudentGradeRow::setGrade(int grade) {
    m_grade = grade;
}
