#ifndef STUDENTGRADEROW_H
#define STUDENTGRADEROW_H

#include <QString>

class StudentGradeRow {
public:
    StudentGradeRow();
    StudentGradeRow(int studentId, const QString &studentFullName, int grade);

    int studentId() const;
    QString studentFullName() const;
    int grade() const;

    void setStudentId(int id);
    void setStudentFullName(const QString &fullName);
    void setGrade(int grade);

private:
    int m_studentId;
    QString m_studentFullName;
    int m_grade;
};

#endif // STUDENTGRADEROW_H
