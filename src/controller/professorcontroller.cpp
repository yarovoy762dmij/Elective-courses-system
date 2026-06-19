#include "professorcontroller.h"
#include "model/dbmanager.h"
#include <QSqlQuery>

QList<Professor> ProfessorController::getAll()
{
    QList<Professor> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM преподаватели ORDER BY id;");
    while (q.next()) {
        list.append(Professor::fromQuery(q));
    }
    return list;
}
