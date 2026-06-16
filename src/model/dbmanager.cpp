#include "dbmanager.h"
#include <QSqlError>
#include <QDebug>


DBManager& DBManager::instance() {
    static DBManager inst;
    return inst;
}

DBManager::~DBManager() {
    disconnectFromDatabase();
}

bool DBManager::connectToDatabase(const QString& host, int port, const QString& dbName, const QString& user, const QString& password) {
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qDebug() << "Ошибка БД:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DBManager::disconnectFromDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

QSqlQuery DBManager::execQuery(const QString& queryString, const QVariantList& params) {
    QSqlQuery query(m_db);
    query.prepare(queryString);

    for (const auto& param : params) {
        query.addBindValue(param);
    }

    query.exec();
    return query;
}
