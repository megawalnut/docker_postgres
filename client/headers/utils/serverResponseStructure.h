#ifndef SERVERRESPONSESTRUCTURE_H
#define SERVERRESPONSESTRUCTURE_H

#include <QString>
#include <QVariantList>

#include "packetStructure.h"
#include "../DBHelpers/dbcrud.h"

using Operation = DBCRUD::Operation;

namespace ServerResponseKeys {
    inline const QString Result = "result";
    inline const QString TableName = "tableName";
    inline const QString Tables = "tables";
    inline const QString UserName = "userName";
    inline const QString Changes = "changes";
    inline const QString State = "state";
    inline const QString Snapshot = "snapshot";
    inline const QString Users = "users";
    inline const QString UserId = "userId";
}

namespace ServerResponseStructure {
    enum class Status {
        Unknown = 0,
        Success,
        Failed
    };
    struct Sync {
        QString tableName;
        Status result;

        // | -Insern- | -Delete- | -Change-
        // Insern - rowId
        // Delete - rowId
        // Change - rowId, colId, fieldVal
        QList<Operation> changes;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(ServerResponseKeys::TableName).toString();
            result = static_cast<Status>(map.value(ServerResponseKeys::Result).toInt());

            changes.clear();
            QVariantList listChange = map.value(ServerResponseKeys::Changes).toList();
            for(const auto& change : listChange) {
                Operation op;
                op.fromMap(change.toMap());
                changes.append(op);
            }
        }
    };
    struct Rollback {
        QString tableName;
        PacketStructure::BulkInsert snapshot;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(ServerResponseKeys::TableName).toString();
            snapshot.fromMap(map.value(ServerResponseKeys::Snapshot).toMap());
        }
    };
    struct Auth {
        int userId;
        QString userName;
        QSet<QString> tables;
        Status result;

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerResponseKeys::UserId).toInt();
            userName = map.value(ServerResponseKeys::UserName).toString();
            result = static_cast<Status>(map.value(ServerResponseKeys::Result).toInt());

            tables.clear();
            for(const auto& name : map.value(ServerResponseKeys::Tables).toStringList()) {
                tables.insert(name);
            };
        }
    };
    struct FullDump {
        QStringList users;
        QList<PacketStructure::BulkInsert> tables;

        void fromMap(const QVariantMap& map) {
            users = map.value(ServerResponseKeys::Users).toStringList();
            tables.clear();

            QVariantList list = map.value(ServerResponseKeys::Tables).toList();
            for (const auto& item : list) {
                PacketStructure::BulkInsert t;
                t.fromMap(item.toMap());
                tables.append(t);
            }
        }
    };
}


#endif // SERVERRESPONSESTRUCTURE_H
