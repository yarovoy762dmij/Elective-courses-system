#include "student.h"

Student Student::fromQuery(const QSqlQuery& q)
{
    Student s;
    s.id = q.value("id").toInt();
    s.lastName = q.value("фамилия").toString();
    s.firstName = q.value("имя").toString();
    s.middleName = q.value("отчество").toString();
    s.phone = q.value("телефон").toString();
    s.address = q.value("адрес").toString();
    s.minSubjectCount = q.value("минимальное_количество_предметов").toInt();
    return s;
}
