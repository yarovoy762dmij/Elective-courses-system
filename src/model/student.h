#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class Student {                                     //студенты
public:
    int id = 0;
    QString lastName;
    QString firstName;
    QString middleName;
    QString phone;
    QString address;
    int minSubjectCount = 0;

    static Student fromQuery(const QSqlQuery& q);
    static QList<Student> getAll();
    static bool insert(const Student& student);
    static bool remove(int id);
};

#endif // STUDENT_H
