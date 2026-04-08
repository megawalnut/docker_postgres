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
            m_controller->onFullDumpFromServer({});
            QMessageBox::information(
                    this,
                    isLogin ? "Login success" : "Registry success",
                    QString("Welcome %1!").arg(name),
                    QMessageBox::Button::Ok
                    );
            m_stack->setCurrentIndex(static_cast<int>(Pages::Main));
    });

    connect(m_controller, &Controller::logout,
            this, [this]() {
            QMessageBox::critical(
                    this,
                    "Error",
                    QString("Error!"
                    "Couldn't find the user!"
                    "Check your username and password!"),
                    QMessageBox::Button::Ok
                    );
    });
}
