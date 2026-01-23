#include "dbhelper.h"

DBHelper::DBHelper(const QString& userName,
                    const QString& hostName,
                    const QString& dbName,
                    const QString& password) {

    QString uniqueName = QString("User_%1")
                                 .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));

    m_db = QSqlDatabase::addDatabase("QPSQL", uniqueName);

    m_db.setDatabaseName(dbName);
    m_db.setHostName(hostName);
    m_db.setUserName(userName);
    m_db.setPassword(password);
    m_db.setPort(5432);

    if(!m_db.open()) {
        qDebug() << QString("DBHelper::Failed to open %1: %2")
                            .arg(m_db.hostName())
                            .arg(m_db.lastError().text());
    } else {
        qDebug() << QString("DBHelper::Open %1")
                            .arg(m_db.hostName());
    }
}

void DBHelper::connect() {
    if(!m_db.open()) {
        qDebug() << QString("DBHelper::Failed to open %1: %2")
                            .arg(m_db.hostName())
                            .arg(m_db.lastError().text());
        return;
    }
    qDebug() << QString("DBHelper::Open %1")
                        .arg(m_db.hostName());
}

DbResult DBHelper::send(const QString& request) const {
    QSqlQuery query(m_db);

    if(!query.exec(request)) {
        qDebug() << QString("DBHelper::The request cannot be completed %1")
                            .arg(query.lastError().text());
        return {false, {}};
    }

    std::vector<QSqlRecord> rec;

    while(query.next()) {
        rec.push_back(query.record());
    }

    return {true, rec};
}

DBHelper::~DBHelper() {
    if(m_db.isOpen()) {
        m_db.close();
        qDebug() << "DBHelper::Database is closed";
    }
}
