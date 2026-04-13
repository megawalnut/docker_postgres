#include "dbcrud.h"
#include "../appContext.h"

DBCRUD::DBCRUD() : m_db(AppContext::instance().db()) {}
// -------------------------------------- CRUD ---------------------------------------------
bool DBCRUD::insert(const Insert& data) {
    if(data.tableName.isEmpty())
    {
        qWarning() << "DBCRUD::insert: Missing fields";
        return false;
    }

    const auto& client = AppContext::instance();

    //check the privileges
    if(!client.currentUser.tables.contains(data.tableName)) {
        qWarning() << "DBCRUD::insert: Permission denied";
        return false;
    }

    QStringList setParts;
    for (const auto& col : data.columns) {
        setParts << "?";
    }

    QString sql = QString("INSERT INTO %1 (%2) "
                          "VALUES (%3)").arg(data.tableName, data.columns.join(", "), setParts.join(", "));

    auto [success, _] = m_db.send(sql, data.values);

    if(!success) {
        qWarning() << "DBCRUD::insert: Query failed";
    }

    return success;
}
bool DBCRUD::change(const Change& data) {
    if(data.tableName.isEmpty() ||
        data.rowId == -1)
    {
        qWarning() << "DBCRUD::change: Missing fields";
        return false;
    }

    const auto& client = AppContext::instance();

    //check the privileges
    if(!client.currentUser.tables.contains(data.tableName)) {
        qWarning() << "DBCRUD::change: Permission denied";
        return false;
    }

    QStringList setParts;
    for (const auto& col : data.columns) {
        setParts << col + " = ?";
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE id = ?").arg(data.tableName, setParts.join(", "));

    auto [success, _] = m_db.send(sql, data.values);

    if (!success) {
        qWarning() << "DBCRUD::change: Query failed";
    }

    return success;
}
bool DBCRUD::remove(const Remove& data) {
    if(data.tableName.isEmpty() ||
        data.rowId == -1)
    {
        qWarning() << "DBCRUD::remove: Missing fields";
        return false;
    }

    const auto& client = AppContext::instance();

    //check the privileges
    if(!client.currentUser.tables.contains(data.tableName)) {
        qWarning() << "DBCRUD::remove: Permission denied";
        return false;
    }

    QString sql = QString("DELETE FROM %1 WHERE id=?").arg(data.tableName);

    auto [success, _] = m_db.send(sql, {data.rowId});

    if (!success) {
        qWarning() << "DBCRUD::remove: Query failed";
    }

    return success;
}
bool DBCRUD::clear(const Clear& data, bool isFullDump) {
    if(data.tableName.isEmpty()) {
        qWarning() << "DBCRUD::clear: Missing fields";
        return false;
    }

    const auto& client = AppContext::instance();

    //check the privileges
    //skip privileges, only for full dump
    //fulldump calling only after connectiong/reconnecting
    if(!client.currentUser.tables.contains(data.tableName) && !isFullDump) {
        qWarning() << "DBCRUD::clear: Permission denied";
        return false;
    }

    QString sql = QString("DELETE FROM %1").arg(data.tableName);

    auto [success, _] = m_db.send(sql, {});

    if (!success) {
        qWarning() << "DBCRUD::clear: Query failed";
    }

    return success;
}
bool DBCRUD::bulkInsert(const BulkInsert& data, bool isFullDump) {
    if(data.tableName.isEmpty() ||
        data.rows.isEmpty() ||
        data.columns.isEmpty())
    {
        qWarning() << "DBCRUD::bulkInsert: Missing fields";
        return false;
    }

    const auto& client = AppContext::instance();

    //check the privileges
    //skip privileges, only for full dump
    if(!client.currentUser.tables.contains(data.tableName) && !isFullDump) {
        qWarning() << "DBCRUD::bulkInsert: Permission denied";
        return false;
    }

    auto db = QSqlDatabase::database();

    if (!db.transaction()) {
        qWarning() << "DBCRUD::bulkInsert: Start transaction failed";
        return false;
    }

    for (const auto& row : data.rows) {
        QStringList setParts;
        for (const auto& col : data.columns) {
            Q_UNUSED(col);
            setParts << "?";
        }

        QString sql = QString("INSERT INTO %1 (%2) "
                              "VALUES (%3)").arg(data.tableName, data.columns.join(", "), setParts.join(", "));

        auto [success, _] = m_db.send(sql, row);

        if (!success) {
            qWarning() << "DBCRUD::bulkInsert: Insert failed";
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        qWarning() << "DBCRUD::bulkInsert: Commit failed";
        return false;
    }

    return true;
}
// -----------------------------------------------------------------------------------------
// -------------------------------------- OTHER --------------------------------------------
DBCRUD::SyncState DBCRUD::getStatus() const {
    return m_currentStatus;
}
void DBCRUD::setStatus(SyncState status) {
    m_currentStatus = static_cast<SyncState>(status);
}
void DBCRUD::clearChanges() {
    m_listChanges.clear();
}
void DBCRUD::addChange(const Operation& data) {
    m_listChanges.append(data);
}
QList<DBCRUD::Operation> DBCRUD::getChanges() const {
    return m_listChanges;
}
// -----------------------------------------------------------------------------------------
