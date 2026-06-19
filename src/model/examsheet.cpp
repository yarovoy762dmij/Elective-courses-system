#include "examsheet.h"

ExamSheet ExamSheet::fromQuery(const QSqlQuery& q)
{
    ExamSheet es;
    es.id = q.value("id").toInt();
    es.creationDate = q.value("дата_создания").toDate();
    es.semesterCourseId = q.value("id_семестрового_курса").toInt();
    es.professorId = q.value("id_преподавателя").toInt();
    return es;
}
