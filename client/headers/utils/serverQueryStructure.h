#ifndef SERVERQUERYSTRUCTURE_H
#define SERVERQUERYSTRUCTURE_H

#include <QString>
#include <QVariantList>

#include "../DBHelpers/dbcrud.h"

using Operation = DBCRUD::Operation;

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

        // | -Insern- | -Delete- | -Change-
        // Insern - rowId
        // Delete - rowId
        // Change - rowId, colId, fieldVal
        QList<Operation> changes;

        QVariantMap toMap() const {
            QVariantMap map;
            map[ServerQueryKeys::UserId] = userId;
            map[ServerQueryKeys::TableName] = tableName;

            QVariantList list;
            for(const auto& change : changes) {
                list.append(change.toMap());
            }
            map[ServerQueryKeys::Changes] = list;
            return map;
        }
    };
    struct Rollback {
        int userId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return {
                {ServerQueryKeys::UserId, userId},
                {ServerQueryKeys::TableName, tableName}
            };
        }
    };
    struct Auth {
        QString userName;
        QString userPass;

        QVariantMap toMap() const {
            return {
                {ServerQueryKeys::UserName, userName},
                {ServerQueryKeys::UserPass, userPass}
            };
        }
    };
    //struct FullDump - we post only opCode
}

#endif  // SERVERQUERYSTRUCTURE_H

