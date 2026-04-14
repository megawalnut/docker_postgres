#include <QMessageBox>

#include "../UI/authPage.h"

AuthPage::AuthPage(Controller* controller, QWidget *parent)
    :
    QWidget{parent},
    m_controller{controller}
{
    qDebug() << "AuthPage::AuthPage";
    init();
    setupConnections();
}

void AuthPage::init() {
    qDebug() << "AuthPage::init";
    QVBoxLayout* root = new QVBoxLayout();
    this->setLayout(root);

    QTabWidget* m_tabs = new QTabWidget();
    root->addWidget(m_tabs);

    //SIGN IN
    {
        QWidget* loginTab = new QWidget();
        QVBoxLayout* login = new QVBoxLayout(loginTab);

        {//QLabel
            QLabel* label = new QLabel("Sign In");
            label->setAlignment(Qt::AlignTop | Qt::AlignCenter);
            label->setFixedHeight(40);
            login->addWidget(label);
        }

        {//username
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);
            row->setAlignment(Qt::AlignCenter);

            QLabel* name = new QLabel("username");
            m_loginUser = new QLineEdit();

            name->setFixedWidth(60);
            m_loginUser->setFixedWidth(150);

            row->addWidget(name);
            row->addWidget(m_loginUser);

            login->addLayout(row);
        }

        {//password
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);
            row->setAlignment(Qt::AlignCenter);

            QLabel* password = new QLabel("password");
            m_loginPass = new QLineEdit();
            m_loginPass->setEchoMode(QLineEdit::Password);

            password->setFixedWidth(60);
            m_loginPass->setFixedWidth(150);

            row->addWidget(password);
            row->addWidget(m_loginPass);

            login->addLayout(row);
        }

        {//button
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);

            m_loginBtn = new QPushButton("Ok");
            m_loginBtn->setFixedWidth(60);

            row->addWidget(m_loginBtn);
            login->addLayout(row);
        }

        m_tabs->addTab(loginTab, "Sign In");
    }

    //SIGN UP
    {
        QWidget* registerTab = new QWidget();
        QVBoxLayout* registry = new QVBoxLayout(registerTab);

        {//QLabel
            QLabel* label = new QLabel("Sign Up");
            label->setAlignment(Qt::AlignTop | Qt::AlignCenter);
            label->setFixedHeight(40);
            registry->addWidget(label);
        }

        {//username
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);
            row->setAlignment(Qt::AlignCenter);

            QLabel* name = new QLabel("username");
            m_regUser = new QLineEdit();

            name->setFixedWidth(60);
            m_regUser->setFixedWidth(150);

            row->addWidget(name);
            row->addWidget(m_regUser);

            registry->addLayout(row);
        }

        {//password
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);
            row->setAlignment(Qt::AlignCenter);

            QLabel* password = new QLabel("password");
            m_regPass = new QLineEdit();

            password->setFixedWidth(60);
            m_regPass->setFixedWidth(150);
            m_regPass->setEchoMode(QLineEdit::Password);

            row->addWidget(password);
            row->addWidget(m_regPass);

            registry->addLayout(row);
        }

        {//button
            QHBoxLayout* row = new QHBoxLayout();
            row->setContentsMargins(0, 0, 0, 0);

            m_regBtn = new QPushButton("Ok");
            m_regBtn->setFixedWidth(60);

            row->addWidget(m_regBtn);
            registry->addLayout(row);
        }

        m_tabs->addTab(registerTab, "Sign Up");
        m_tabs->setCurrentIndex(static_cast<int>(TabPages::SignIn));
    }
}

void AuthPage::setupConnections() {
    //login strat connection to server
    connect(m_loginBtn, &QPushButton::clicked,
            this, [this]() { m_controller->connectToServer(); });

    //registry strat connection to server
    connect(m_regBtn, &QPushButton::clicked,
            this, [this]() { m_controller->connectToServer(); });

    connect(m_controller, &Controller::connected,
            this, [this]() {
        qDebug() << "Start auth...";
        m_controller->onAuth(m_loginUser->text(), m_loginPass->text(), m_tabs->currentIndex() == 0); });
}
