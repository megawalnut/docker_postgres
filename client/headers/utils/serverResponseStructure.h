#ifndef SERVERRESPONSESTRUCTURE_H
#define SERVERRESPONSESTRUCTURE_H

#include <QString>
#include <QVariantList>

#include "packetQueryStructure.h"

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
        Success = 0,
        Failed
    };
    struct Sync {
        QString tableName;
        QVariantList changes;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerResponseKeys::TableName, tableName},
                {ServerResponseKeys::Changes, changes}
            };
        }

        void fromMap(const QVariantMap& map) {
            tableName = map.value(ServerResponseKeys::TableName).toString();
            changes = map.value(ServerResponseKeys::Changes).toList();
        }
    };
    struct Rollback {
        QString tableName;
        PacketStructure::BulkInsert snapshot;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerResponseKeys::TableName, tableName},
                {ServerResponseKeys::Snapshot, snapshot.toMap()}
            };
        }

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

        QVariantMap toMap() const {
            QStringList list = QStringList(tables.cbegin(), tables.cend());
            return QVariantMap {
                {ServerResponseKeys::UserId, userId},
                {ServerResponseKeys::UserName, userName},
                {ServerResponseKeys::Tables, list},
                {ServerResponseKeys::Result,  static_cast<int>(result)}
            };
        }

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

        QVariantMap toMap() const {
            QVariantList tablesList;

            for (const auto& t : tables) {
                tablesList.append(t.toMap());
            }

            return QVariantMap {
                {ServerResponseKeys::Users, users},
                {ServerResponseKeys::Tables, tablesList}
            };
        }

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
