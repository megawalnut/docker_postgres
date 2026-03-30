#include "../UI/authPage.h"

AuthPage::AuthPage(QWidget *parent) : QWidget{parent} {
    qDebug() << "AuthPage::AuthPage";
    init();
    setupConnections();
}

void AuthPage::init() {
    qDebug() << "AuthPage::init";
    QVBoxLayout* root = new QVBoxLayout();
    this->setLayout(root);

    QTabWidget* tabs = new QTabWidget();
    root->addWidget(tabs);

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

        tabs->addTab(loginTab, "Sign In");
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

        tabs->addTab(registerTab, "Sign Up");
        tabs->setCurrentIndex(static_cast<int>(TabPages::SignIn));
    }
}

void AuthPage::setupConnections() {
    qDebug() << "AuthPage::setupConnections";
    connect(m_loginBtn, &QPushButton::clicked,
            this, [this]() { emit loginRequested(m_loginUser->text(), hashPassword(m_loginPass->text())); });
    connect(m_regBtn, &QPushButton::clicked,
            this, [this]() { emit registerRequested(m_regUser->text(), hashPassword(m_regPass->text())); });
}

QString AuthPage::hashPassword(const QString& password) const {
    qDebug() << "AuthPage::hashPassword";
    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hashedPassword.toHex());
}
