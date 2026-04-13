#include "controller.h"
#include "utils.h"
#include "../appContext.h"

#include "../DBHelpers/dbcrud.h"

Controller::Controller(Client* client,
                       TableModel* table,
                       const QStringList& users,
                       QObject *parent)
    :
    QObject(parent),
    m_client(client),
    m_db(std::make_unique<DBCRUD>())
{
    qDebug() << "Controller::Controller";
    setupConnections();
    setTableModel(table);
    setUsersModel(users);
}

// ------------------------------- COMMANDS FROM SERVER ------------------------------------
void Controller::onPacketReady(QByteArray rawData) {
    auto [opcode, packet] = Utils::Packet::deserialize(rawData);
    parseServerCommand(opcode, packet);
}
// --------------------------- COMMANDS FROM SERVER + TO UI -----------------------
void Controller::onSyncFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onSyncFromServer";
    if (!m_client) {
        qWarning() << "Controller::onSyncFromServer: Invalid client";
        return;
    }

    ServerResponseStructure::Sync data;
    data.fromMap(packet);

    m_db->clearChanges();

    switch(data.result) {
    case ServerResponseStructure::Status::Success: {
        m_db->setStatus(SyncState::Synced);

        emit syncSuccess();
        emit statusChanged();
        qWarning() << "Controller::onSyncFromServer: Success";
        return;
    }
    case ServerResponseStructure::Status::Failed: {
        m_db->setStatus(SyncState::Unsynced);

        for (const auto& c : data.changes) {
            m_db->addChange(c);
        }

        emit statusChanged();
        qWarning() << "Controller::onSyncFromServer: Failed";
        return;
    }
    case ServerResponseStructure::Status::Unknown:
    default: {
        qWarning() << "Controller::onSyncFromServer: Invalid case" << static_cast<int>(data.result);
        return;
    }
    }

}
void Controller::onRollbackFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onRollbackFromServer";
    if (!m_client) {
        qWarning() << "Controller::onRollbackFromServer: Invalid client";
        return;
    }

    ServerResponseStructure::Rollback data;
    data.fromMap(packet);

    m_db->clear(Clear{data.tableName});
    m_db->bulkInsert(data.snapshot);
    m_db->clearChanges();
    m_db->setStatus(SyncState::Synced);

    emit statusChanged();
}
void Controller::onAuthFromServer(const QVariantMap& packet, bool isLogin) {
    qDebug() << "Controller::onAuthFromServer";

    ServerResponseStructure::Auth data;
    data.fromMap(packet);

    switch(data.result) {
    case ServerResponseStructure::Status::Success: {

        auto& client = AppContext::instance().currentUser;

        client.userId = data.userId;
        client.userName = data.userName;
        client.tables = data.tables;

        onFullDump();

        emit currentUserChanged(client.userName);
        emit statusChanged();
        emit authSuccess(isLogin, client.userName);
        qDebug() << "Controller::onAuthFromServer: Success";
        return;
    }
    case ServerResponseStructure::Status::Failed: {
        emit logout();
        qWarning() << "Controller::onAuthFromServer: Logout";
        return;
    }
    case ServerResponseStructure::Status::Unknown:
    default: {
        qWarning() << "Controller::onAuthFromServer: Invalid case" << static_cast<int>(data.result);
        return;
    }
    }
}
void Controller::onFullDumpFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onFullDumpFromServer";
    if (!m_client) {
        qWarning() << "Controller::onFullDumpFromServer: Invalid client";
        return;
    }

    ServerResponseStructure::FullDump data;
    data.fromMap(packet);

    m_users = data.users;

    for(const auto& table : data.tables) {
        m_db->clear(Clear{table.tableName}, true);
        m_db->bulkInsert(table, true);
    }

    m_db->clearChanges();
    m_db->setStatus(SyncState::Synced);

    emit statusChanged();
}
// -----------------------------------------------------------------------------------------
// -------------------------------- COMMANDS TO SERVER -------------------------------------
void Controller::connectToServer() {
    qDebug() << "Controller::setupConnections";
    m_client->connection();
}
void Controller::onSync() {
    qDebug() << "Controller::onSync";
    if (!m_client) {
        qWarning() << "Controller::onSync: Invalid client";
        return;
    }

    const auto& listChanges = m_db->getChanges();

    if (listChanges.isEmpty()) {
        qWarning() << "Controller::onSync: List changes is empty";
        return;
    }

    ServerQueryStructure::Sync data;
    data.userId = AppContext::instance().currentUser.userId;
    data.tableName = m_currentTable->getTableName();
    data.changes = listChanges;

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Sync), data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onRollback() {
    qDebug() << "Controller::onRollback";
    if (!m_client) {
        qWarning() << "Controller::onRollback: Invalid client";
        return;
    }

    ServerQueryStructure::Rollback data;
    data.userId = AppContext::instance().currentUser.userId;
    data.tableName = m_currentTable->getTableName();

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Rollback), data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onAuth(const QString userName, const QString rawPass, bool isLogin) {
    qDebug() << "Controller::onAuth";
    if (!m_client) {
        qWarning() << "Controller::onAuth: Invalid client";
        return;
    }

    ServerQueryStructure::Auth data;
    data.userName = userName;
    data.userPass = hashPassword(rawPass);

    const uint32_t opcode = isLogin ? static_cast<uint32_t>(ServerOpcode::Login)
                                    : static_cast<uint32_t>(ServerOpcode::Registry);

    QByteArray packet = Utils::Packet::serialize(opcode, data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onFullDump() {
    qDebug() << "Controller::onSignUp";
    if (!m_client) {
        qWarning() << "Controller::onSignUp: Invalid client";
        return;
    }
    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::FullDump), {});

    m_client->sendPacket(packet);
}
// -----------------------------------------------------------------------------------------
// ----------------------------- LOCAL COMMANDS FROM UI ------------------------------------
void Controller::onInsertRow() {
    qDebug() << "Controller::onInsertRow";
    if (!m_currentTable) {
        qWarning() << "Controller::onInsertRow: Invalid current table";
        return;
    }

    int id = m_currentTable->insertRow();
    emit dataChanged();

    m_db->addChange({ OperationsEnum::InsertEnum, QVariantMap{ {ControllerKeys::Id, id} } });
    m_db->setStatus(SyncState::Unsynced);

    emit statusChanged();
}
void Controller::onDeleteRow(int selectedRow) {
    qDebug() << "Controller::onDeleteRow";
    if (!m_currentTable) {
        qWarning() << "Controller::onDeleteRow: Invalid current table";
        return;
    }

    if (selectedRow < 0 || selectedRow >= m_currentTable->rowCount()) {
        qWarning() << "Controller::onDeleteRow: Invalid selected row";
        return;
    }

    int id = m_currentTable->deleteRow(selectedRow);
    emit dataChanged();

    m_db->addChange({ OperationsEnum::DeleteEnum, QVariantMap{{ControllerKeys::Id, id}} });
    m_db->setStatus(SyncState::Unsynced);

    emit statusChanged();
}
void Controller::onChangeField(int selectedRow, int selectedCol, QVariant value) {
    qDebug() << "Controller::onChangeField";
    if (!m_currentTable) {
        qWarning() << "Controller::onChangeField: Invalid current table";
        return;
    }

    if (selectedRow < 0 || selectedRow >= m_currentTable->rowCount()) {
        qWarning() << "Controller::onChangeField: Invalid selected row";
        return;
    }

    if (selectedCol < 0 || selectedCol >= m_currentTable->columnCount()) {
        qWarning() << "Controller::onChangeField: Invalid selected column";
        return;
    }

    const QVariantMap& data = m_currentTable->changeField(selectedRow, selectedCol, value);
    emit dataChanged();

    m_db->addChange({ OperationsEnum::ChangeEnum, data });
    m_db->setStatus(SyncState::Unsynced);

    emit statusChanged();
}
void Controller::onUpdateTable(const QString userName) {
    qDebug() << "Controller::onUpdateTable";
    if (!m_currentTable) {
        qWarning() << "Controller::onUpdateTable: Invalid current table";
        return;
    }

    if (!m_tables.contains(userName)) {
        qWarning() << "Table not found:" << userName;
        return;
    }

    if (m_currentTable == m_tables[userName]) {
        qWarning() << "Controller::onUpdateTable: Current table == set table";
        return;
    }

    m_currentTable = m_tables.value(userName);

    emit currentModelChanged(m_currentTable);
    emit currentModelNameChanged(m_currentTable->getTableName());
    emit statusChanged();
}
// -----------------------------------------------------------------------------------------
// ---------------------------------- OTHER PRIVATE ----------------------------------------
void Controller::setupConnections() {
    if (!m_client) {
        qWarning() << "Controller::setupConnections: Invalid client";
        return;
    }
    //read from socket
    connect(m_client, &Client::packetReady,
            this, &Controller::onPacketReady);
    connect(m_client, &Client::serverConnected,
            this, [this]() {
        m_db->setStatus(SyncState::Synced);
        emit connected();
    });
    connect(m_client, &Client::serverError,
            this, [this]() {
        m_db->setStatus(SyncState::Unknown);
        emit error();
    });
    connect(m_client, &Client::serverDisconnected,
            this, [this]() {
        m_db->setStatus(SyncState::Unknown);
        emit disconnected();
    });

}
void Controller::parseServerCommand(ServerOpcode command, const QVariantMap& packet) {
    switch(command){
    case ServerOpcode::Login: {
        qDebug() << "Controller:ServerOpcode::Login";
        onAuthFromServer(packet, true);
        return;
    }
    case ServerOpcode::Registry: {
        qDebug() << "Controller:ServerOpcode::Registry";
        onAuthFromServer(packet, false);
        return;
    }
    case ServerOpcode::Sync: {
        qDebug() << "Controller:ServerOpcode::Sync";
        onSyncFromServer(packet);
        return;
    }
    case ServerOpcode::Rollback: {
        qDebug() << "Controller:ServerOpcode::Rollback";
        onRollbackFromServer(packet);
        return;
    }
    case ServerOpcode::FullDump: {
        qDebug() << "Controller:ServerOpcode::FullDump";
        onFullDumpFromServer(packet);
        return;
    }
    case ServerOpcode::Unknown:
    default: {
        qWarning() << "Controller::ServerOpcode: Unknown opcode" << static_cast<int>(command);
        return;
    }
    }
}
QString Controller::hashPassword(const QString& password) const {
    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashedPassword.toHex());
}
// -----------------------------------------------------------------------------------------
// ---------------------------------- OTHER PUBLIC -----------------------------------------
SyncState Controller::getStatus() const {
    return m_db->getStatus();
}
void Controller::setTableModel(TableModel* data) {
    if (!data) {
        qWarning() << "Controller::setTableModel: Invalid pointer";
        return;
    }
    m_currentTable = data;

    m_db->setStatus(SyncState::Synced);

    emit currentModelChanged(m_currentTable);
    emit statusChanged();
}
void Controller::setUsersModel(const QStringList& data) {
    m_users = data;

    emit usersChanged(m_users);
}
void Controller::setCurrentUser(const QString& name) {
    AppContext::instance().currentUser.userName = name;

    m_db->setStatus(SyncState::Synced);
    emit currentUserChanged(AppContext::instance().currentUser.userName);
    emit statusChanged();
}
// -----------------------------------------------------------------------------------------
