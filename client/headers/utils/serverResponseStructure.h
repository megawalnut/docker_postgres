#ifndef SERVERRESPONSESTRUCTURE_H
#define SERVERRESPONSESTRUCTURE_H

#include <QString>
#include <QVariantList>
#include <QVariantList>

#include "packetQueryStructure.h"

namespace ServerResponseKeys {
    inline const QString Result = "result";
    inline const QString TableName = "tableName";
    inline const QString Tables = "tables";
    inline const QString UserName = "userName";
    inline const QString UserId = "userId";
    inline const QString Changes = "changes";
    inline const QString State = "state";
}

namespace ServerResponseStructure {
    enum class Status {
        Success = 0,
        Failed
    };
    enum class SyncStatus {
        Insert = 0,
        Update,
        Remove,
        Clear,
        BulkInsert
    };
    struct Sync {
        SyncStatus syncStatus;
        std::variant<PacketStructure::Insert,
                     PacketStructure::Update,
                     PacketStructure::Remove,
                     PacketStructure::Clear,
                     PacketStructure::BulkInsert> changes;

        QVariantMap toMap() const {
            qDebug() << "ServerResponseStructure::Sync:toMap";
            return QVariantMap {
                {ServerResponseKeys::State, static_cast<int>(syncStatus)},
                {ServerResponseKeys::Changes, changes}
            };
        };

        void fromMap(const QVariantMap& map) {
            qDebug() << "ServerResponseStructure::Sync:fromMap";
            syncStatus = static_cast<SyncStatus>(map.value(ServerResponseKeys::State).toInt());
            changes = map.value(ServerResponseKeys::Changes).toMap();
        };
    };
    struct Rollback {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerResponseKeys::UserId, userId},
                {ServerResponseKeys::TableName, tableName}
            };
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerResponseKeys::UserId).toInt();
            tableName = map.value(ServerResponseKeys::TableName).toString();
        };
    };
    struct Auth {
        int userId = -1;
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
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerResponseKeys::UserId).toInt();
            userName = map.value(ServerResponseKeys::UserName).toString();
            result = static_cast<Status>(map.value(ServerResponseKeys::Result).toInt());

            tables.clear();
            for(const auto& name : map.value(ServerResponseKeys::Tables).toStringList()) {
                tables.insert(name);
            };
        };
    };
    struct FullDump {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerResponseKeys::UserId, userId},
                {ServerResponseKeys::TableName, tableName}
            };
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerResponseKeys::UserId).toInt();
            tableName = map.value(ServerResponseKeys::TableName).toString();
        };
    };
    struct Users {

    };
}


#endif // SERVERRESPONSESTRUCTURE_H
