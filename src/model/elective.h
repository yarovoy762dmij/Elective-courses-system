#ifndef ELECTIVE_H
#define ELECTIVE_H

#include <QString>
#include <QSqlQuery>

class Elective {
public:
    int id = 0;
    QString name;

    static Elective fromQuery(const QSqlQuery& q);
};

#endif // ELECTIVE_H
