#include "fulldump.h"
#include "Utils/keys.h"
#include "Utils/protocol.h"

QVariantMap FullDump::exec(const QVariantMap& data, ClientConnection* client) {
    //ClientQueryStructure::FullDump query;
    //data is empty! Just opcode!

    ServerResponseStructure::FullDump responce;

    const auto& ptr_db = client->db();
    if(!ptr_db) {
        qWarning() << "Login::exec: Pointer to database is empy";
        return responce.toMap();
    }

    QString sql = "SELECT user_name, table_name FROM users";
    auto [success, res] = ptr_db->send(sql, {});
    if (!success || res.empty()) {
        qWarning() << "FullDump::exec: Failed SELECT users";
        return responce.toMap();
    }

    QStringList userNames;
    QStringList tableNames;
    QList<PacketStructure::BulkInsert> tables;

    //--------------------USERS----------------------
    for(const auto& row : res) {
        userNames.append(row.value(Keys::UserInfo_UserName).toString());
        tableNames.append(row.value(Keys::UserInfo_TableName).toString());
    }

    //--------------------TABLES---------------------
    for(const auto& table : tableNames) {
        PacketStructure::BulkInsert snapshot;
        snapshot.tableName = table;

        QString sql = QString("SELECT * FROM %1").arg(table);
        auto [success, res] = ptr_db->send(sql, {});
        if (!success) {
            qWarning() << "FullDump::exec: Failed SELECT " << table;
            continue;
        }

        snapshot.columns = {"ID", "Type", "Name", "Brand", "Model", "Year"};

        if (res.empty()) {
            //table is empty
            tables.append(snapshot);
            continue;
        }

        const QSqlRecord& firstRow = res[0];

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
        tables.append(snapshot);
    }

    responce.users = userNames;
    responce.tables = tables;

    qDebug() << "FullDump::exec: Success";

    return responce.toMap();
}


