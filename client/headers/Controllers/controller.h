#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVariantMap>

#include "client.h"
#include "serverOpcode.h"
#include "localOpcode.h"
#include "tableModel.h"
#include "dbcrud.h"
#include "protocol.h"

using LocalOpcode = LocalOpcode;
using ServerOpcode = ServerOpcode;
using SyncState = DBCRUD::SyncState;
using OperationsEnum = Operations::OperationsEnum;

namespace ControllerKeys {
    inline const QString Id = "id";
    inline const QString Operation = "operation";
}

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(Client* client, TableModel* table, const QStringList& users, QObject *parent = nullptr);

    SyncState getStatus() const;    //Sync/Unsync/Unkn from DBCRUD
    void setTableModel(TableModel* data);
    void setUsersModel(const QStringList& data);
    void setCurrentUser(const QString& name);

private:
    void setupConnections();
    void parseServerCommand(ServerOpcode command, const QVariantMap& packet);   //parse opcode from server
    QString hashPassword(const QString& password) const;    //hashing pass

public slots:
    //get answer from server
    void onPacketReady(QByteArray data);

    //update UI
    void onUpdateTable(const QString name);

    //table methods
    void onInsertRow();
    void onDeleteRow(int selectedRow);
    void onChangeField(int selectedRow, int selectedCol, QVariant value);

    //send command on server
    void connectToServer();
    void onSync();
    void onRollback();
    void onAuth(const QString userName, QString hashPass, bool isLogin);
    void onFullDump();

    //receive command from server
    void onSyncFromServer(const QVariantMap& packet);
    void onRollbackFromServer(const QVariantMap& packet);
    void onAuthFromServer(const QVariantMap& packet, bool isLogin);   //slot for auth from server
    void onFullDumpFromServer(const QVariantMap& packet);

signals:
    //signals from server connection
    void connected();
    void error();
    void disconnected();

    //signals from server connection
    void syncSuccess();
    void syncError();

    void sendServer(const QByteArray& data);
    void dataChanged();

    //update UI models
    void usersChanged(const QStringList users);
    void currentModelChanged(TableModel* table);

    //update UI states
    void currentUserChanged(const QString currentUser);
    void currentModelNameChanged(QString name);

    void statusChanged();

    void authSuccess(bool isLogin, const QString name);
    void logout();

private:
    Client* m_client = nullptr;
    QHash<QString, TableModel*> m_tables;   //users tables
    TableModel* m_currentTable = nullptr;   //current user table
    QStringList m_users;    //users tables
    std::unique_ptr<DBCRUD> m_db = nullptr; //cache
};

#endif // CONTROLLER_H
