#ifndef TEACHINGLOAD_H
#define TEACHINGLOAD_H

#include <QList>
#include <QSqlQuery>

class TeachingLoad {                                    //обучение_студента
public:
    int id = 0;
    int hoursCount = 0;
    int lessonTypeId = 0;
    int semesterCourseId = 0;
    int teacherId = 0;

    static TeachingLoad fromQuery(const QSqlQuery& q);
    static QList<TeachingLoad> getAll();
    static bool insert(const TeachingLoad& load);
    static bool remove(int id);
};

#endif // TEACHINGLOAD_H
