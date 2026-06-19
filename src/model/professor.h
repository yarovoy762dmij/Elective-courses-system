#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class Professor {
public:
    int id = 0;
    QString lastName;
    QString firstName;
    QString middleName;
    QString phone;
    QString position;
    int userId = 0;

    static Professor fromQuery(const QSqlQuery& q);
    static QList<Professor> getAll();
};

#endif // PROFESSOR_H
