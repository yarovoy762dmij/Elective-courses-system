#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantList>
#include <QString>

class DBManager {
public:
    //Единственная точка доступа (синглтон Майерса)
    static DBManager& instance();

    bool connectToDatabase(const QString& host, int port, const QString& dbName, const QString& user, const QString& password);
    void disconnectFromDatabase();

    QSqlQuery execQuery(const QString& queryString, const QVariantList& params = QVariantList());

private:
    DBManager() = default;
    ~DBManager();
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
