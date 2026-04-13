#include <QMessageBox>

#include "mainwindow.h"

MainWindow::MainWindow(Controller* controller, QWidget *parent)
    :
    QMainWindow(parent),
    m_controller(controller),
    m_authPage(new AuthPage(controller)),
    m_mainPage(new MainPage(controller))
{
    qDebug() << "MainWindow::MainWindow";

    init();
    setupConnections();
}

void MainWindow::init() {
    qDebug() << "MainWindow::init";

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_stack->addWidget(m_authPage);
    m_stack->addWidget(m_mainPage);

    m_stack->setCurrentIndex(static_cast<int>(Pages::Auth));
}

void MainWindow::setupConnections() {
    qDebug() << "MainWindow::setupConnections";

    connect(m_controller, &Controller::authSuccess,
            this, [this](bool isLogin, const QString name) {
        QMessageBox::information(
            this,
            isLogin ? "Login success" : "Registry success",
            QString("Welcome %1!").arg(name),
            QMessageBox::Button::Ok);
        m_stack->setCurrentIndex(static_cast<int>(Pages::Main));
    });

    connect(m_controller, &Controller::logout,
            this, [this]() {
        QMessageBox::critical(
            this,
            "Error",
            "Error!""Couldn't find the user!\n"
            "Check your username and password!",
            QMessageBox::Button::Ok);
    });

    connect(m_controller, &Controller::disconnected,
            this, [this]() {
        QMessageBox::critical(
            this,
            "Connection Lost",
            "Connection to the server was lost.\n"
            "Please try again later.",
            QMessageBox::Ok);
    });

    connect(m_controller, &Controller::error,
            this, [this]() {
        QMessageBox::critical(
            this,
            "Connection Error",
            "Failed to connect to the server.\n"
            "Check your internet connection or server status.",
            QMessageBox::Ok);
    });
}
