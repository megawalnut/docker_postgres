#include "dbhelper.h"

DBHelper::DBHelper(const QString& hostName,
                   const QString& dbName,
                   const QString& dbUser,
                   const QString& dbPassword)
{
    m_connectionName = QString("DB_%1")
                           .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));

    m_db = QSqlDatabase::addDatabase("QPSQL", m_connectionName);

    m_db.setHostName(hostName);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(dbUser);
    m_db.setPassword(dbPassword);
    m_db.setPort(PORTDB);
}

//maybe unused
void DBHelper::connect() {
    if(!m_db.open()) {
        qDebug() << QString("DBHelper::Failed to open %1: %2")
                            .arg(m_db.hostName())
                            .arg(m_db.lastError().text());
    } else {
        qDebug() << QString("DBHelper::DBHelper: Opened database file %1")
                        .arg(m_db.hostName());
    }
    return;
}

DbResult DBHelper::send(const QString& request,  const QVariantList& values) {
    if (!m_db.isOpen()) {
        qWarning() << "DB not open";
        return {false, {}};
    }

    QSqlQuery query(m_db);

    query.prepare(request);
    for (const auto& v : values)
        query.addBindValue(v);

    if(!query.exec()) {
        qWarning() << QString("DBHelper::send: The request cannot be completed %1")
                          .arg(query.lastError().text());
        return {false, {}};
    }

    std::vector<QSqlRecord> rec;

    if(query.isSelect()) {
        while(query.next()) {
            rec.push_back(query.record());
        }
    }

    return {true, rec};
}

DBHelper::~DBHelper() {
    if(m_db.isOpen()) {
        m_db.close();
        qDebug() << "DBHelper::Database is closed";
    }
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
}
