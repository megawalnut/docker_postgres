#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVariantMap>

#include "client.h"

#include "serverOpcode.h"
#include "localOpcode.h"

#include "utils/serverQueryStructure.h"
#include "utils/serverResponseStructure.h"

#include "../models/tableModel.h"
#include "../DBHelpers/dbcrud.h"


using namespace ServerQueryStructure;
using namespace ServerResponseStructure;


using LocalOpcode = LocalOpcode;
using ServerOpcode = ServerOpcode;

class Controller : public QObject {
    Q_OBJECT
public:
    struct Operation {
        LocalOpcode op;
        QVariantMap data;
    };

    explicit Controller(Client* client, TableModel* table, const QStringList& users, QObject *parent = nullptr);

    void setTableModel(TableModel* data);
    void setUsersModel(const QStringList& data);
    void setCurrentUser(const QString& name);

    int getStatus() const;

private:
    void setupConnections();
    void parseServerCommand(ServerOpcode command, const QVariantMap& packet);
    void loginOrReg(const QVariantMap& packet, bool isLogin);
    QString hashPassword(const QString& password) const;

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
    void onSync();
    void onRollback();
    void onSignIn(const QString userName, QString hashPass);
    void onSignUp(const QString userName, QString hashPass);

    //receive command from server
    void onLoginFromServer(const QVariantMap& packet);
    void onRegistryFromServer(const QVariantMap& packet);
    void onSyncFromServer(const QVariantMap& packet);
    void onRollbackFromServer(const QVariantMap& packet);
    void onFullDumpFromServer(const QVariantMap& packet);

signals:
    void sendServer(const QByteArray& data);
    void dataChanged();

    //update UI models
    void usersChanged(const QStringList users);
    void currentModelChanged(TableModel* table);

    //update UI states
    void currentUserChanged(const QString currentUser);
    void currentModelNameChanged(QString name);

    void statusChanged();
    void tableNameChanged();

    void authSuccess(bool isLogin, const QString name);
    void logout();

private:
    Client* m_client = nullptr;

    TableModel* m_currentTable = nullptr;
    QStringList m_users;

    QString m_currentTableName;

    QList<Operation> m_listChanges;

    QHash<QString, TableModel*> m_tables;
    std::unique_ptr<DBCRUD> m_db = nullptr;
};

#endif // CONTROLLER_H
