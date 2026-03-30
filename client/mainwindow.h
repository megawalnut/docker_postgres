#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../UI/authPage.h"
#include "../UI/mainPage.h"

#include "../models/tableModel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum class Pages {
        Auth = 0,
        Main,
    };

    MainWindow(QWidget *parent = nullptr);
    void init();

    void setupModels();
    void setupConnections();

private:
    void onLoginSuccess(const QString user);

private:
    QString m_currentUser = "user1";
    QList<QString> m_users = {"user1", "user2", "user3"};
    TableModel* m_tableData;

    QStackedWidget* m_stack;
    AuthPage* m_authPage;
    MainPage* m_mainPage;
};
#endif // MAINWINDOW_H
