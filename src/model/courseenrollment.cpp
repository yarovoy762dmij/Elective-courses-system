#include "courseenrollment.h"
#include "dbmanager.h"
#include <QVariant>

CourseEnrollment CourseEnrollment::fromQuery(const QSqlQuery& q) {
    CourseEnrollment ce;
    ce.electiveId = q.value("id_факультатива").toInt();
    ce.studentId = q.value("id_студента").toInt();
    if (q.value("итоговая_оценка").isNull())
        ce.finalGrade = std::nullopt;
    else
        ce.finalGrade = q.value("итоговая_оценка").toInt();
    return ce;
}

QList<CourseEnrollment> CourseEnrollment::getAll() {
    QList<CourseEnrollment> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM прохождения_курсов "
        "ORDER BY id_факультатива, id_студента;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool CourseEnrollment::insert(int electiveId, int studentId, std::optional<int> finalGrade) {
    QVariantList params = {electiveId, studentId};
    QString sql = "INSERT INTO прохождения_курсов (id_факультатива, id_студента, итоговая_оценка) "
                  "VALUES (?, ?, ?);";
    params.append(finalGrade.has_value() ? QVariant(*finalGrade) : QVariant());
    QSqlQuery q = DBManager::instance().execQuery(sql, params);
    return q.isActive();
}

bool CourseEnrollment::remove(int electiveId, int studentId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM прохождения_курсов "
        "WHERE id_факультатива = ? AND id_студента = ?;",
        {electiveId, studentId});
    return q.isActive();
}
