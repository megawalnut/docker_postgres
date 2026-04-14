#include "sync.h"
#include "Utils/protocol.h"
#include "Utils/keys.h"

QVariantMap Sync::exec(const QVariantMap& data, ClientConnection* client) {
    ClientQueryStructure::Sync query;
    query.fromMap(data);

    ServerResponseStructure::Sync response;
    response.result = ServerResponseStructure::Status::Success;
    response.rejected.clear();

    const auto& ptr_db = client->db();
    if(!ptr_db) {
        qWarning() << "Login::exec: Pointer to database is empy";
        return response.toMap();
    }

    bool allOk = true;

    for (const auto& change : query.changes) {
        QString table = change.data[Keys::UserInfo_TableName].toString();
        if (table.isEmpty()) {
            qWarning() << "Sync::exec: Empty table name";
            allOk = false;
            continue;
        }

        switch (change.op) {
        // ---------------- INSERT ----------------
        case Operations::OperationsEnum::InsertEnum: {
            int id = change.data[Keys::ServerResponse_Id].toInt();

            QString sql = QString("INSERT INTO %1 (id) VALUES (?)")
                              .arg(table);

            auto [success, _] = ptr_db->send(sql, {id});
            if (!success) {
                qWarning() << "Sync::exec: Failed INSERT " << table;
                response.rejected.append(change);
                allOk = false;
            }

            break;
        }
        // ---------------- DELETE ----------------
        case Operations::OperationsEnum::DeleteEnum: {
            int id = change.data[Keys::ServerResponse_Id].toInt();

            QString sql = QString("DELETE FROM %1 WHERE id=?")
                              .arg(table);

            auto [success, _] = ptr_db->send(sql, {id});
            if (!success) {
                qWarning() << "Sync::exec: Failed DELETE " << table;
                response.rejected.append(change);
                allOk = false;
            }

            break;
        }
        // ---------------- CHANGE ----------------
        case Operations::OperationsEnum::ChangeEnum: {
            QString column = change.data[Keys::ServerResponse_Columns].toString();
            QVariant value = change.data[Keys::ServerResponse_Value];
            int id = change.data[Keys::ServerResponse_Id].toInt();

            if (column.isEmpty()) {
                qWarning() << "Sync::exec: Empty column";
                response.rejected.append(change);
                allOk = false;
                break;
            }

            QString sql = QString("UPDATE %1 SET %2=? WHERE id=?")
                              .arg(table).arg(column);

            auto [success, _] = ptr_db->send(sql, {value, id});
            if (!success) {
                qWarning() << "Sync::exec: Failed UPDATE " << table;
                allOk = false;
            }

            break;
        }

        default: {
            qWarning() << "Sync::exec: Invalid opcode" << static_cast<int>(change.op);
            allOk = false;
            break;
        }
        }
    }

    response.result = allOk ? ServerResponseStructure::Status::Success
                            : ServerResponseStructure::Status::Failed;

    qDebug() << "Sync::exec: Finished!Applied changes: " << query.changes.size() << "/" << query.changes.size();
    return response.toMap();
}


