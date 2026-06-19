#include "elective.h"

Elective Elective::fromQuery(const QSqlQuery& q)
{
    Elective e;
    e.id = q.value("id").toInt();
    e.name = q.value("название_факультатива").toString();
    return e;
}
