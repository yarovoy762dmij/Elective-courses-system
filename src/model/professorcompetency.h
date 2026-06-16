#ifndef PROFESSORCOMPETENCY_H
#define PROFESSORCOMPETENCY_H

#include <QList>
#include <QSqlQuery>

class ProfessorCompetency {                                       //компетенции_преподавателей
public:
    int electiveId = 0;
    int teacherId = 0;

    static ProfessorCompetency fromQuery(const QSqlQuery& q);
    static QList<ProfessorCompetency> getAll();
    static bool insert(int electiveId, int teacherId);
    static bool remove(int electiveId, int teacherId);
};

#endif // PROFESSORCOMPETENCY_H
