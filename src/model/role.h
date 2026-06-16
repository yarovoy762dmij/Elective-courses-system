#ifndef ROLE_H
#define ROLE_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class Role {                                    //роли (справочник без методов записи и удаления)
public:
    int id = 0;
    QString name;

    static Role fromQuery(const QSqlQuery& q);
    static QList<Role> getAll();
};

#endif // ROLE_H
