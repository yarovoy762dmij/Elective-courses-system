#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QSqlQuery>

class Student {
public:
    int id = 0;
    QString lastName;
    QString firstName;
    QString middleName;
    QString phone;
    QString address;
    int minSubjectCount = 0;

    static Student fromQuery(const QSqlQuery& q);
};

#endif // STUDENT_H
