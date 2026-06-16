#ifndef ENTITIES_H
#define ENTITIES_H

#include <QString>
#include <QList>
#include <QSqlQuery>


struct User {                                       //Пользователь
    int id = 0;
    QString login;
    QString email;
    QString roleName;

    static User fromQuery(const QSqlQuery& q);
};


struct Elective {                                   //Факультатив
    int id = 0;
    QString name;

    static QList<Elective> getAll();                //CRUD-методы, которые возвращают объекты
    static bool insert(const QString& name);
    static bool remove(int id);
};


#endif // ENTITIES_H
