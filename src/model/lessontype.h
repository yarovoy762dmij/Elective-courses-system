#ifndef LESSONTYPE_H
#define LESSONTYPE_H

#include <QString>
#include <QList>
#include <QSqlQuery>

class LessonType {                                      //виды_занятия (справочник без методов записи и удаления)
public:
    int id = 0;
    QString name;

    static LessonType fromQuery(const QSqlQuery& q);
    static QList<LessonType> getAll();
};

#endif // LESSONTYPE_H
