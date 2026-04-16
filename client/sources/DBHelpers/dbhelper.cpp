#include "dbhelper.h"

DBHelper::DBHelper(const QString& filePath) : m_path(filePath) {
    m_connectionName = QString("User_%1")
                             .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(m_path);

    if(!m_db.open()) {
        qWarning() << QString("DBHelper::DBHelper: Failed to open %1: %2")
                            .arg(m_path)
                            .arg(m_db.lastError().text());

        qFatal("DB open failed");
    } else {
        qDebug() << QString("DBHelper::DBHelper: Opened database file %1")
                            .arg(m_path);
    }
}

DbResult DBHelper::send(const QString& request,  const QVariantList& values) {
    if (!m_db.isOpen()) {
        qWarning() << "DB not open";
        return {false, {}};
    }

    QSqlQuery query(m_db);

    bool ok = false;

    if (values.isEmpty()) {
        ok = query.exec(request);
    } else {
        query.prepare(request);
        for (const auto& v : values)
            query.addBindValue(v);
        ok = query.exec();
    }

    if (!ok) {
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
        qWarning() << "DBHelper::send: Database is closed";
    }
    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
}
