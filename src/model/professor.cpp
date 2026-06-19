#include "professor.h"
#include "controller/professorcontroller.h"

Professor Professor::fromQuery(const QSqlQuery& q)
{
    Professor t;
    t.id = q.value("id").toInt();
    t.lastName = q.value("фамилия").toString();
    t.firstName = q.value("имя").toString();
    t.middleName = q.value("отчество").toString();
    t.phone = q.value("телефон").toString();
    t.position = q.value("должность").toString();
    t.userId = q.value("id_пользователя").toInt();
    return t;
}

QList<Professor> Professor::getAll()
{
    return ProfessorController::getAll();
}
