#ifndef USER_H
#define USER_H

#include <QString>
#include <QSqlQuery>

class User {
public:
    int id = 0;
    QString login;
    QString email;
    QString roleName;

    static User fromQuery(const QSqlQuery& q);
};

#endif // USER_H
