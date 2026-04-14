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

        void fromMap(const QVariantMap& map) {
            changes.clear();
            QVariantList list = map.value(Keys::Operation_Changes).toList();
            for(const auto& item : list) {
                Operations::Operation op;
                op.fromMap(item.toMap());
                changes.append(op);
            }
        }
    };
    struct Auth {
        UserInfo user;
        QString userPass;

        void fromMap(const QVariantMap& map) {
            user.fromMap(map.value(Keys::UserInfo_UserInfo).toMap());
            userPass = map.value(Keys::ClientQuery_UserPass).toString();
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

        QVariantMap toMap() const {
            QVariantMap map;
            map[Keys::ServerResponse_Result] = static_cast<int>(result);

            QVariantList list;
            for(const auto& change : rejected) {
                list.append(change.toMap());
            }
            map[Keys::Operation_Changes] = list;
            return map;
        }
    };
    struct Rollback {
        PacketStructure::BulkInsert snapshot;

        QVariantMap toMap() const {
            QVariantMap map;
            map[Keys::UserInfo_TableName] = snapshot.tableName;
            map[Keys::ServerResponse_Columns] = snapshot.columns;

            QVariantList temp;
            for(const auto& row : snapshot.rows) {
                temp.append(row);
            }
            map[Keys::ServerResponse_Rows] = temp;
            return map;
        }
    };
    struct Auth {
        UserInfo user;
        Status result;

        QVariantMap toMap() const {
            QVariantMap map = user.toMap();
            map[Keys::ServerResponse_Result] = static_cast<int>(result);
            return map;
        }
    };
    struct FullDump {
        QStringList users;
        QList<PacketStructure::BulkInsert> tables;

        QVariantMap toMap() const {
            QVariantMap map;
            map[Keys::ServerResponse_Users] = users;

            QVariantList list;
            for(const auto& table: tables) {
                QVariantMap snapshot;
                snapshot[Keys::UserInfo_TableName] = table.tableName;
                snapshot[Keys::ServerResponse_Columns] = table.columns;

                QVariantList temp;
                for(const auto& row : table.rows) {
                    temp.append(row);
                }
                snapshot[Keys::ServerResponse_Rows] = temp;

                list.append(snapshot);
            }

            map[Keys::ServerResponse_Tables] = list;
            return map;
        }
    };
}

#endif // PROTOCOL_H
