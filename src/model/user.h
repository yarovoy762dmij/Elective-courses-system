#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class User {                                    // пользователи (+ роль через JOIN для наглядности)
public:
    int id = 0;
    QString login;
    QString email;
    QString roleName;

    static User fromQuery(const QSqlQuery& q);
    static QList<User> getAll();
};

#endif // USER_H
