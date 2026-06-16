#include "examrecord.h"
#include "dbmanager.h"

ExamRecord ExamRecord::fromQuery(const QSqlQuery& q) {
    ExamRecord er;
    er.sheetId = q.value("id_ведомости").toInt();
    er.studentId = q.value("id_студента").toInt();
    er.grade = q.value("оценка").toInt();
    return er;
}

QList<ExamRecord> ExamRecord::getAll() {
    QList<ExamRecord> list;
    QSqlQuery q = DBManager::instance().execQuery(
        "SELECT * FROM записи_в_ведомостях "
        "ORDER BY id_ведомости, id_студента;");
    while (q.next())
        list.append(fromQuery(q));
    return list;
}

bool ExamRecord::insert(int sheetId, int studentId, int grade) {
    QSqlQuery q = DBManager::instance().execQuery(
        "INSERT INTO записи_в_ведомостях (id_ведомости, id_студента, оценка) "
        "VALUES (?, ?, ?);",
        {sheetId, studentId, grade});
    return q.isActive();
}

bool ExamRecord::remove(int sheetId, int studentId) {
    QSqlQuery q = DBManager::instance().execQuery(
        "DELETE FROM записи_в_ведомостях "
        "WHERE id_ведомости = ? AND id_студента = ?;",
        {sheetId, studentId});
    return q.isActive();
}
