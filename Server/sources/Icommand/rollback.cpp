#include "rollback.h"
#include "Utils/protocol.h"

QVariantMap Rollback::exec(const QVariantMap& data, ClientConnection* client) {
    //ClientQueryStructure::Rollback query;
    //data is empty! Just opcode!

    UserInfo info = client->getUser();

    ServerResponseStructure::Auth responce;

    const auto& ptr_db = client->db();
    if(!ptr_db) {
        qWarning() << "Login::exec: Pointer to database is empy";
        return responce.toMap();
    }

    if (info.tableName.isEmpty()) {
        qWarning() << "Rollback::exec: Invalid data";
        return responce.toMap();
    }

    QString sql = QString("SELECT * FROM %1").arg(info.tableName);
    auto [success, res] = ptr_db->send(sql, {});
    if (!success || res.empty()) {
        qWarning() << "Rollback::exec: Failed SELECT " << info.tableName;
        return responce.toMap();
    }

    const QSqlRecord& firstRow = res[0];

    PacketStructure::BulkInsert snapshot;
    snapshot.tableName = info.tableName;

    //cols
    for (int col = 0; col < firstRow.count(); ++col) {
        snapshot.columns.append(firstRow.fieldName(col));
    }

    //rows
    for (const auto& row : res) {
        QVariantList values;
        for (int i = 0; i < row.count(); ++i) {
            values.append(row.value(i));
        }
        snapshot.rows.append(values);
    }

    qDebug() << "Rollback::exec: Success " << info.tableName;

    return responce.toMap();
}

