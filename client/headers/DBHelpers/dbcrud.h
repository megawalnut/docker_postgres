#ifndef DBCRUD_H
#define DBCRUD_H

#include <QString>

#include "dbhelper.h"
#include "protocol.h"

namespace DBCRUDKeys {
inline const QString Operation = "operation";
inline const QString Data = "data";
}

class DBCRUD {
public:
    enum class SyncState {
        Unsynced = 0,
        Synced = 1,
        Unknown = -1
    };

    explicit DBCRUD();

    bool insert(const PacketStructure::Insert& data);
    bool change(const PacketStructure::Change& data);
    bool remove(const PacketStructure::Remove& data);
    bool clear(const PacketStructure::Clear& data, bool isFullDump = false);
    bool bulkInsert(const PacketStructure::BulkInsert& data, bool isFullDump = false);

    DBCRUD::SyncState getStatus() const;
    void setStatus(SyncState status);
    void clearChanges();
    void addChange(const Operations::Operation& data);
    QList<Operations::Operation> getChanges() const;

private:
    DBHelper& m_db;
    QList<Operations::Operation> m_listChanges;
    SyncState m_currentStatus = SyncState::Unknown;
};

#endif // DBCRUD_H
