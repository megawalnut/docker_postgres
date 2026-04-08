#ifndef SERVERQUERYSTRUCTURE_H
#define SERVERQUERYSTRUCTURE_H

#include <QString>
#include <QVariantList>
#include <QVariantList>

#include "packetQueryStructure.h"

namespace ServerQueryKeys {
    inline const QString UserId = "userId";
    inline const QString TableName = "tableName";
    inline const QString Changes = "changes";
    inline const QString UserName = "userName";
    inline const QString UserPass = "userPass";
}

namespace ServerQueryStructure {
    struct Sync {
        int userId = -1;
        QString tableName;
        std::variant<PacketStructure::Insert,
                     PacketStructure::Update,
                     PacketStructure::Remove,
                     PacketStructure::Clear,
                     PacketStructure::BulkInsert> changes;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName},
                {ServerQueryKeys::Changes, changes.toMap()}
            };
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerQueryKeys::UserId).toInt();
            tableName = map.value(ServerQueryKeys::TableName).toString();
            changes = map.value(ServerQueryKeys::Changes).toList();
        };
    };
    struct Rollback {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName}
            };
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerQueryKeys::UserId).toInt();
            tableName = map.value(ServerQueryKeys::TableName).toString();
        };
    };
    struct Auth {
        QString userName;
        QString userPass;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserName, userName},
                {ServerQueryKeys::UserPass, userPass}
            };
        };

        void fromMap(const QVariantMap& map) {
            userName = map.value(ServerQueryKeys::UserName).toString();
            userPass = map.value(ServerQueryKeys::UserPass).toString();
        };
    };
    struct FullDump {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName}
            };
        };

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerQueryKeys::UserId).toInt();
            tableName = map.value(ServerQueryKeys::TableName).toString();
        };
    };
    struct Users {

    };
}

#endif  // SERVERQUERYSTRUCTURE_H

