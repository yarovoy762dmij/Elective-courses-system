#ifndef EXAMRECORD_H
#define EXAMRECORD_H

#include <QList>
#include <QSqlQuery>

class ExamRecord {                                          //записи_в_ведомости
public:
    int sheetId = 0;
    int studentId = 0;
    int grade = 0;

    static ExamRecord fromQuery(const QSqlQuery& q);
    static QList<ExamRecord> getAll();
    static bool insert(int sheetId, int studentId, int grade);
    static bool remove(int sheetId, int studentId);
};

#endif // EXAMRECORD_H
