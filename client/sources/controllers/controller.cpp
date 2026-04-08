#include <QMessageBox>

#include "controller.h"
#include "utils.h"
#include "../appContext.h"

Controller::Controller(Client* client,
                       TableModel* table,
                       const QStringList& users,
                       QObject *parent)
    :
    QObject{parent},
    m_client(client),
    m_db(std::make_unique<DBCRUD>(PATH))
{
    qDebug() << "Controller::Controller";

    setupConnections();
    setTableModel(table);
    setUsersModel(users);
}

// ------------------------------- COMMANDS FROM SERVER ------------------------------------
void Controller::onPacketReady(QByteArray rawData) {
    qDebug() << "Controller::onPacketReady";
    auto [opcode, packet] = Utils::Packet::deserialize(rawData);
    parseServerCommand(static_cast<ServerOpcode>(opcode), packet);
}
// -----------------------------------------------------------------------------------------
// -------------------------------- COMMANDS TO SERVER -------------------------------------
void Controller::onSync() {
    qDebug() << "Controller::onSync";
    if (!m_client) {
        qWarning() << "Controller::onSync: Invalid client";
        return;
    }
    if (m_listChanges.isEmpty()) {
        qWarning() << "Controller::onSync: List changes is empty";
        return;
    }

    ServerPacketStructure::Sync data;
    data.userId = AppContext::instance().currentUser.userId;
    data.tableName = m_currentTableName;

    QVariantList changes;
    for(const auto& change : m_listChanges) {
        QVariantMap action = change.data;
        action["op"] = static_cast<uint32_t>(change.op);
        changes.append(action);
    }

    data.changes = changes;

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Sync), data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onRollback() {
    qDebug() << "Controller::onRollback";
    if (!m_client) {
        qWarning() << "Controller::onRollback: Invalid client";
        return;
    }

    ServerPacketStructure::Rollback data;
    data.userId = AppContext::instance().currentUser.userId;
    data.tableName = m_currentTableName;

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Rollback), data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onSignIn(const QString userName, const QString rawPass) {
    qDebug() << "Controller::onSignIn";
    if (!m_client) {
        qWarning() << "Controller::onSignIn: Invalid client";
        return;
    }

    ServerPacketStructure::Auth data;
    data.userName = userName;
    data.userPass = hashPassword(rawPass);

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Login), data.toMap());

    m_client->sendPacket(packet);
}
void Controller::onSignUp(const QString userName, const QString rawPass) {
    qDebug() << "Controller::onSignUp";
    if (!m_client) {
        qWarning() << "Controller::onSignUp: Invalid client";
        return;
    }

    ServerPacketStructure::Auth data;
    data.userName = userName;
    data.userPass = hashPassword(rawPass);

    QByteArray packet = Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::Registry), data.toMap());

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

    m_listChanges.append({ LocalOpcode::Insert, QVariantMap{{"id", id}} });
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

    m_listChanges.append({ LocalOpcode::Delete, QVariantMap{{"id", id}} });
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

    m_listChanges.append({ LocalOpcode::Change, data });
    emit statusChanged();
}
void Controller::onUpdateTable(const QString userName) {
    qDebug() << "Controller::onUpdateTable";
    if (!m_currentTable) {
        qWarning() << "Controller::onUpdateTable: Invalid current table";
        return;
    }

    if (m_currentTable == m_tables[userName]) {
        qWarning() << "Controller::onUpdateTable: Current table == set table";
        return;
    }

    m_currentTable = m_tables[userName];
    m_currentTableName = m_currentTable->getTableName();

    emit currentModelChanged(m_currentTable);
    emit currentModelNameChanged(m_currentTableName);
    emit statusChanged();
}
// -----------------------------------------------------------------------------------------
// ------------------------------ LOCAL COMMANDS TO UI -------------------------------------
void Controller::onFullDumpFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onFullDumpFromServer";
    m_db->clear({{"table", "users"}});

    QVariantList data = packet["data"].toList();
    m_db->bulkInsert("users", data);

    m_listChanges.clear();
    emit statusChanged();
}
void Controller::onRollbackFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onRollbackFromServer";
    m_db->clear({{"table", "users"}});

    QVariantList snapshot = packet["snapshot"].toList();
    m_db->bulkInsert("users", snapshot);

    m_listChanges.clear();
    emit statusChanged();
}
void Controller::onSyncFromServer(const QVariantMap& packet) {
    qDebug() << "Controller::onSyncFromServer";
    QVariantList changes = packet["changes"].toList();

    for (const auto& c : changes) {
        QVariantMap change = c.toMap();

        QString type = change["type"].toString();

        if (type == "insert")
            m_db->insert(change);

        else if (type == "update")
            m_db->update(change);

        else if (type == "delete")
            m_db->remove(change);
    }

    m_listChanges.clear();
    emit statusChanged();
}
// -----------------------------------------------------------------------------------------
// ---------------------------------- OTHER PRIVATE ----------------------------------------
void Controller::setupConnections() {
    qDebug() << "Controller::setupConnections";
    if (!m_client) {
        qWarning() << "Controller::setupConnections: Invalid client";
        return;
    }
    //read from socket
    connect(m_client, &Client::packetReady,
            this, &Controller::onPacketReady);
}
void Controller::parseServerCommand(ServerOpcode command, const QVariantMap& packet) {
    qDebug() << "Controller::parseServerCommand";
    switch(command){
    case ServerOpcode::Login: {
        qDebug() << "Controller:ServerOpcode::Login";
        loginOrReg(packet, true);
        return;
    }
    case ServerOpcode::Registry: {
        qDebug() << "Controller:ServerOpcode::Registry";
        loginOrReg(packet, false);
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
void Controller::loginOrReg(const QVariantMap& packet, bool isLogin) {
    qDebug() << "Controller::loginOrReg";

    ServerResponseStructure::Auth data;
    data.fromMap(packet);

    switch(data.result) {
    case ServerResponseStructure::Status::Success: {

        auto& client = AppContext::instance().currentUser;

        client.userId = data.userId;
        client.userName = data.userName;
        client.tables = data.tables;

        emit currentUserChanged(client.userName);
        emit authSuccess(isLogin, client.userName);
        emit statusChanged();
        qDebug() << "Controller::loginOrReg: Success";
        break;
    }
    case ServerResponseStructure::Status::Failed: {
        emit logout();
        qWarning() << "Controller::loginOrReg: Logout";
        break;
    }
    default: {
        qWarning() << "Controller::loginOrReg: Invalid case";
        break;
    }
    }
    return;
}
QString Controller::hashPassword(const QString& password) const {
    qDebug() << "Controller::hashPassword";
    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashedPassword.toHex());
}
// -----------------------------------------------------------------------------------------
// ---------------------------------- OTHER PUBLIC -----------------------------------------
void Controller::setTableModel(TableModel* data) {
    qDebug() << "Controller::setTableModel";
    if (!data) {
        qWarning() << "Controller::setTableModel: Invalid pointer";
        return;
    }
    m_currentTable = data;

    emit currentModelChanged(m_currentTable);
    emit statusChanged();
}
void Controller::setUsersModel(const QStringList& data) {
    qDebug() << "Controller::setTableModel";
    m_users = data;

    emit usersChanged(m_users);
}
void Controller::setCurrentUser(const QString& name) {
    qDebug() << "Controller::setTableModel";
    m_currentUser = name;

    emit currentUserChanged(m_currentUser);
    emit statusChanged();
}
int Controller::getStatus() const {
    qDebug() << "Controller::setTableModel";
    return m_currentTable ? static_cast<int>(m_currentTable->getSyncState())
                          : static_cast<int>(AppContext::SyncState::Unknown);
}
// -----------------------------------------------------------------------------------------
