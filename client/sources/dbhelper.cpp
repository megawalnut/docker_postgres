#include "dbhelper.h"

DBHelper::DBHelper(const QString& filePath) {
    m_path = filePath;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_path);


    if(!m_db.open()) {
        qDebug() << QString("DBHelper::Failed to open %1: %2")
                            .arg(m_path)
                            .arg(m_db.lastError().text());
    } else {
        qDebug() << QString("DBHelper::Opened database file %1")
                            .arg(m_path);
    }
}

DbResult DBHelper::send(const QString& request,  const QVariantList& values) {
    QSqlQuery query(m_db);

    query.prepare(request);
    for (const auto& v : values)
        query.addBindValue(v);

    if(!query.exec()) {
        qDebug() << QString("DBHelper::The request cannot be completed %1")
                            .arg(query.lastError().text());
        return {false, {}};
    }

    std::vector<QSqlRecord> rec;

    bool isSelect = request.trimmed().startsWith("SELECT", Qt::CaseInsensitive);
    if(isSelect) {
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
}
