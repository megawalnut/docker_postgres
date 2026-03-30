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

class MainPage : public QWidget {
    Q_OBJECT
public:
    explicit MainPage(QWidget *parent = nullptr);
    void setTableModel(TableModel* data);
    void setUserModel(const QList<QString>& data);
    void setCurrentUser(const QString& user);

private:
    void init();
    void setupConnections();
    void setStatus();

public slots:
    void onCurrentUserChanged();

signals:
    void userChanged();

private:
    QString m_currentUserName;

    QTableView* m_tableView = nullptr;
    QComboBox* m_userNames;


    QPushButton* m_rollback;
    QPushButton* m_push;
    QPushButton* m_insert;
    QPushButton* m_delete;
    QPushButton* m_change;


    QLabel* m_currentUser;
    QLabel* m_tableName;
    QLabel* m_status;

    bool dataChanged = false;
    QPair<QString, QString> statuses = {"🟢 Synced" , "🔴 Unsynced"};
    TableModel* m_records = nullptr;
    QList<QString> m_users;
};

#endif // MAINPAGE_H
