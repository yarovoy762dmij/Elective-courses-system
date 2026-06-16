#include "lessontype.h"
#include "dbmanager.h"

LessonType LessonType::fromQuery(const QSqlQuery& q) {
    LessonType lt;
    lt.id = q.value("id").toInt();
    lt.name = q.value("название_вида").toString();
    return lt;
}

QList<LessonType> LessonType::getAll() {
    QList<LessonType> list;
    QSqlQuery q = DBManager::instance().execQuery("SELECT * FROM виды_занятия ORDER BY id;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}
