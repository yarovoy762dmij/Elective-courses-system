#ifndef ELECTIVE_H
#define ELECTIVE_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class Elective {                                    //факультативы
public:
    int id = 0;
    QString name;

    static Elective fromQuery(const QSqlQuery& q);
    static QList<Elective> getAll();
    static bool insert(const QString& name);
    static bool remove(int id);
};

#endif // ELECTIVE_H
