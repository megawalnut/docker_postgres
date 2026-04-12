#ifndef SERVERQUERYSTRUCTURE_H
#define SERVERQUERYSTRUCTURE_H

#include <QString>
#include <QVariantList>

#include "packetQueryStructure.h"

namespace ServerQueryKeys {
    inline const QString UserId = "userId";
    inline const QString TableName = "tableName";
    inline const QString Changes = "changes";
    inline const QString UserName = "userName";
    inline const QString UserPass = "userPass";
    inline const QString Operation = "operation";
}

using PacketStructure::Structures;

namespace ServerQueryStructure {
    struct Sync {
        int userId = -1;
        QString tableName;
        QVariantList changes;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName},
                {ServerQueryKeys::Changes, changes}
            };
        }

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerQueryKeys::UserId).toInt();
            tableName = map.value(ServerQueryKeys::TableName).toString();

            changes.clear();

            QVariantList list = map.value(ServerQueryKeys::Changes).toList();

            for (const auto& item : list) {
                QVariantMap m = item.toMap();

                int op = m.value(ServerQueryKeys::Operation).toInt();

                switch (static_cast<Structures>(op)) {
                case Structures::InsertEnum: {
                    PacketStructure::Insert v;
                    v.fromMap(m);
                    changes.push_back(QVariant::fromValue(v));
                    break;
                }
                case Structures::UpdateEnum: {
                    PacketStructure::Update v;
                    v.fromMap(m);
                    changes.push_back(QVariant::fromValue(v));
                    break;
                }
                case Structures::RemoveEnum: {
                    PacketStructure::Remove v;
                    v.fromMap(m);
                    changes.push_back(QVariant::fromValue(v));
                    break;
                }
                case Structures::ClearEnum: {
                    PacketStructure::Clear v;
                    v.fromMap(m);
                    changes.push_back(QVariant::fromValue(v));
                    break;
                }
                case Structures::BulkInsertEnum: {
                    PacketStructure::BulkInsert v;
                    v.fromMap(m);
                    changes.push_back(QVariant::fromValue(v));
                    break;
                }
                default: {
                    qWarning() << "Unknown operation:" << op;
                    break;
                }
                }
            }
        }
    };
    struct Rollback {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName}
            };
        }

        void fromMap(const QVariantMap& map) {
            userId = map.value(ServerQueryKeys::UserId).toInt();
            tableName = map.value(ServerQueryKeys::TableName).toString();
        }
    };
    struct Auth {
        QString userName;
        QString userPass;

        QVariantMap toMap() const {
            return QVariantMap {
                {ServerQueryKeys::UserName, userName},
                {ServerQueryKeys::UserPass, userPass}
            };
        }

        void fromMap(const QVariantMap& map) {
            userName = map.value(ServerQueryKeys::UserName).toString();
            userPass = map.value(ServerQueryKeys::UserPass).toString();
        }
    };
}

#endif  // SERVERQUERYSTRUCTURE_H

