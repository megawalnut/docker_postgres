#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QVariantList>

#include "keys.h"
#include "userInfo.h"

namespace Operations {
    enum class OperationsEnum : uint32_t {
        UnknownEnum = 0,
        InsertEnum,
        DeleteEnum,
        ChangeEnum
    };

    // | -Insern- | -Delete- | -Change-
    // Insern - rowId
    // Delete - rowId
    // Change - rowId, colId, fieldVal
    struct Operation {
        OperationsEnum op;
        QVariantMap data;

        void fromMap(const QVariantMap& map) {
            op = static_cast<OperationsEnum>(map.value(Keys::Operation_Operation).toUInt());
            data = map.value(Keys::Operation_Data).toMap();
        }

        QVariantMap toMap() const {
            QVariantMap map = data;
            map[Keys::Operation_Operation] =  static_cast<uint32_t>(op);
            return map;
        }
    };
}

namespace PacketStructure {
    struct Insert {
        QString tableName;
        QStringList columns;
        QVariantList values;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(Keys::UserInfo_TableName).toString();
            columns = map.value(Keys::PacketStructure_Columns).toStringList();
            values = map.value(Keys::PacketStructure_Values).toList();
        }
    };
    struct Change {
        int rowId = -1;
        QString tableName;
        QStringList columns;
        QVariantList values;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(Keys::UserInfo_TableName).toString();
            columns = map.value(Keys::PacketStructure_Columns).toStringList();
            values = map.value(Keys::PacketStructure_Values).toList();
            rowId = map.value(Keys::PacketStructure_RowId).toInt();
        }
    };
    struct Remove {
        QString tableName;
        int rowId = -1;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(Keys::UserInfo_TableName).toString();
            rowId = map.value(Keys::PacketStructure_RowId).toInt();
        }
    };
    struct Clear {
        QString tableName;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(Keys::UserInfo_TableName).toString();
        }
    };
    struct BulkInsert { //entire table
        QString tableName;
        QStringList columns;
        QList<QVariantList> rows;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(Keys::UserInfo_TableName).toString();
            columns = map.value(Keys::PacketStructure_Columns).toStringList();

            rows.clear();
            for(const auto& row : map.value(Keys::PacketStructure_Rows).toList()) {
                rows.push_back(row.toList());
            };
        }
    };
}

namespace ClientQueryStructure {
    struct Sync {
        // | -Insern- | -Delete- | -Change-
        // Insern - rowId
        // Delete - rowId
        // Change - rowId, colId, fieldVal
        QList<Operations::Operation> changes;

        QVariantMap toMap() const {
            QVariantMap map;

            QVariantList list;
            for(const auto& change : changes) {
                list.append(change.toMap());
            }
            map[Keys::Operation_Changes] = list;
            return map;
        }
    };
    struct Auth {
        UserInfo user;
        QString userPass;

        QVariantMap toMap() const {
            QVariantMap map = user.toMap();
            map[Keys::ClientQuery_UserPass] = userPass;
            return map;
        }
    };
    //struct Rollback {};   just opcode
    //struct FullDump {};   just opcode
}

namespace ServerResponseStructure {
    enum class Status {
        Unknown = 0,
        Success,
        Failed
    };
    struct Sync {
        // | -Insern- | -Delete- | -Change-
        // Insern - rowId
        // Delete - rowId
        // Change - rowId, colId, fieldVal
        QList<Operations::Operation> rejected;
        Status result;

        void fromMap(const QVariantMap& map) {
            result = static_cast<Status>(map.value(Keys::ServerResponse_Result).toInt());

            rejected.clear();
            QVariantList listChange = map.value(Keys::Operation_Changes).toList();
            for(const auto& change : listChange) {
                Operations::Operation op;
                op.fromMap(change.toMap());
                rejected.append(op);
            }
        }
    };
    struct Rollback {
        PacketStructure::BulkInsert snapshot;

        void fromMap(const QVariantMap& map) {
            snapshot.fromMap(map.value(Keys::ServerResponse_Snapshot).toMap());
        }
    };
    struct Auth {
        UserInfo user;
        Status result;

        void fromMap(const QVariantMap& map) {
            user.userId = map.value(Keys::UserInfo_UserId).toInt();
            user.userName = map.value(Keys::UserInfo_UserName).toString();
            user.tableName = map.value(Keys::UserInfo_TableName).toString();
            result = static_cast<Status>(map.value(Keys::ServerResponse_Result).toInt());
        }
    };
    struct FullDump {
        QStringList users;
        QList<PacketStructure::BulkInsert> tables;

        void fromMap(const QVariantMap& map) {
            users = map.value(Keys::ServerResponse_Users).toStringList();
            tables.clear();

            QVariantList list = map.value(Keys::ServerResponse_Tables).toList();
            for (const auto& item : list) {
                PacketStructure::BulkInsert t;
                t.fromMap(item.toMap());
                tables.append(t);
            }
        }
    };
}

#endif // PROTOCOL_H
