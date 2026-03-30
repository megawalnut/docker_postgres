#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    m_tableData(new TableModel(this)),
    m_authPage(new AuthPage(this)),
    m_mainPage(new MainPage(this)) {

    init();
    setupModels();
    setupConnections();
}

void MainWindow::init() {
    //central stack
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_stack->addWidget(m_authPage);
    m_stack->addWidget(m_mainPage);

    m_stack->setCurrentWidget(m_mainPage);
}

void MainWindow::setupModels() {
    m_mainPage->setTableModel(m_tableData);
    m_mainPage->setCurrentUser(m_currentUser);
    m_mainPage->setUserModel(m_users);
}

void MainWindow::setupConnections() {
    // connect(m_authPage, &Controller::loginSuccess,
    //         this, &MainWindow::onLoginSuccess);
}

void MainWindow::onLoginSuccess(const QString user) {
    m_currentUser = user;
    m_mainPage->setCurrentUser(user);
}
