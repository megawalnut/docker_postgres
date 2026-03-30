#include "../UI/mainPage.h"

MainPage::MainPage(QWidget *parent) : QWidget{parent} {
    qDebug() << "MainPage::MainPage";
    init();
    setMinimumSize(638, 400);
    setupConnections();
}

void MainPage::init() {
    qDebug() << "MainPage::init";
    QVBoxLayout* main = new QVBoxLayout();
    setLayout(main);
    main->setSpacing(5);

    // tabbar
    {
        QHBoxLayout* tabbar = new QHBoxLayout();

        tabbar->addWidget(new QLabel("Current user: "));
        m_currentUser = new QLabel();
        tabbar->addWidget(m_currentUser);

        m_userNames = new QComboBox();
        tabbar->addWidget(m_userNames);

        m_rollback = new QPushButton("Rollback");
        tabbar->addWidget(m_rollback);

        m_push = new QPushButton("Sync");
        tabbar->addWidget(m_push);

        main->addLayout(tabbar);
    }

    // inner block
    QFrame* inner = new QFrame();
    inner->setStyleSheet("background-color: #d9d9d9;");
    QVBoxLayout* columnLayout = new QVBoxLayout(inner);

    // toolbar
    {
        QHBoxLayout* toolbar = new QHBoxLayout();

        toolbar->addWidget(new QLabel("Table: "));

        m_tableName = new QLabel();
        toolbar->addWidget(m_tableName);

        m_insert = new QPushButton("Insert");
        toolbar->addWidget(m_insert);

        m_delete = new QPushButton("Delete");
        toolbar->addWidget(m_delete);

        m_change = new QPushButton("Change");
        toolbar->addWidget(m_change);

        columnLayout->addLayout(toolbar);
    }

    // table
    {
        m_tableView = new QTableView(inner);
        columnLayout->addWidget(m_tableView, 1);
    }

    main->addWidget(inner);

    // status
    {
        QHBoxLayout* taskbar = new QHBoxLayout();

        taskbar->addWidget(new QLabel("Status: "));
        m_status = new QLabel();
        taskbar->addWidget(m_status);
        taskbar->addStretch();

        main->addLayout(taskbar);

        setStatus();
    }
}

void MainPage::setTableModel(TableModel* data) {
    qDebug() << "MainPage::setTableModel";
    m_records = data;
    if (m_tableView && m_records)
        m_tableView->setModel(m_records);
}

void MainPage::setUserModel(const QList<QString>& data) {
    qDebug() << "MainPage::setUserModel";
    m_users = data;
    int index = -1;
    m_userNames->clear();

    for(int i = 0; i < m_users.size(); ++i) {
        m_userNames->addItem(m_users[i]);

        if(m_users[i] == m_currentUserName)
            index = i;
    }
    if(index == -1) {
        m_userNames->setEnabled(false);
    }
    m_userNames->setCurrentIndex(index);
}

void MainPage::setCurrentUser(const QString& user) {
    qDebug() << "MainPage::setCurrentUser";
    if (m_currentUserName != user) {
        m_currentUserName = user;
        m_currentUser->setText(m_currentUserName);

        if(m_userNames->count() > 0)
            m_userNames->setCurrentText(m_currentUserName);
    }
}

void MainPage::setStatus() {
    qDebug() << "MainPage::setStatus";
    if (dataChanged) {
        m_status->setText("🔴 Unsynced");
        m_status->setStyleSheet("color: red;");
        m_push->setStyleSheet("color: red;");
    } else {
        m_status->setText("🟢 Synced");
        m_status->setStyleSheet("color: green;");
        m_push->setStyleSheet("color: green;");
    }
}

void MainPage::setupConnections() {
    qDebug() << "MainPage::setupConnections";
    connect(m_userNames, &QComboBox::currentTextChanged,
            this, &MainPage::onCurrentUserChanged);

}

void MainPage::onCurrentUserChanged() {
    qDebug() << "MainPage::onCurrentUserChanged";
    // change table/ not users
}
