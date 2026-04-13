#ifndef DBCRUD_H
#define DBCRUD_H

#include <QString>

#include "dbhelper.h"
#include "../headers/utils/packetStructure.h"

using namespace PacketStructure;

namespace DBCRUDKeys {
inline const QString Operation = "operation";
inline const QString Data = "data";
}

class DBCRUD {
public:
    enum class OperationsEnum : uint32_t {
        UnknownEnum = 0,
        InsertEnum,
        DeleteEnum,
        ChangeEnum
    };

    enum class SyncState {
        Unsynced = 0,
        Synced = 1,
        Unknown = -1
    };

    // | -Insern- | -Delete- | -Change-
    // Insern - rowId
    // Delete - rowId
    // Change - rowId, colId, fieldVal
    struct Operation {
        OperationsEnum op;
        QVariantMap data;

        void fromMap(const QVariantMap& map) {
            op = static_cast<OperationsEnum>(map.value(DBCRUDKeys::Operation).toUInt());
            data = map.value(DBCRUDKeys::Data).toMap();
        }

        QVariantMap toMap() const {
            QVariantMap map = data;
            map[DBCRUDKeys::Operation] =  static_cast<uint32_t>(op);
            return map;
        }
    };

    explicit DBCRUD();

    bool insert(const Insert& data);
    bool change(const Change& data);
    bool remove(const Remove& data);
    bool clear(const Clear& data, bool isFullDump = false);
    bool bulkInsert(const BulkInsert& data, bool isFullDump = false);

    DBCRUD::SyncState getStatus() const;
    void setStatus(SyncState status);
    void clearChanges();
    void addChange(const Operation& data);
    QList<Operation> getChanges() const;

private:
    DBHelper& m_db;
    QList<Operation> m_listChanges;
    SyncState m_currentStatus = SyncState::Unknown;
};

#endif // DBCRUD_H
