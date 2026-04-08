#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTableView>
#include <QPair>

#include "../models/tableModel.h"
#include "controller.h"
#include "../../appContext.h"

class MainPage : public QWidget {
    Q_OBJECT
    using SyncState = AppContext::SyncState;
public:
    explicit MainPage(Controller* controller, QWidget *parent = nullptr);
    void setTableModel(TableModel* data);
    void setUserModel(const QStringList& data);
    void setCurrentUser(const QString& user);

private:
    void init();
    void setupConnections();
    void setStatus(AppContext::SyncState status);
    void setStatusUser();

private:
    //current user
    QLabel* m_currentUser = nullptr;
    QString m_currentUserName;

    //users model
    QComboBox* m_userNames = nullptr;
    QList<QString> m_users;

    //table model
    QLabel* m_tableName;
    QTableView* m_tableView = nullptr;
    TableModel* m_records = nullptr;

    //buttons
    QPushButton* m_rollback = nullptr;
    QPushButton* m_sync = nullptr;
    QPushButton* m_insert = nullptr;
    QPushButton* m_delete = nullptr;
    QPushButton* m_change = nullptr;

    //status
    QLabel* m_status = nullptr;
    QPair<QString, QString> statuses = {"🟢 Synced" , "🔴 Unsynced"};

    //controller
    Controller* m_controller = nullptr;
};

#endif // MAINPAGE_H
