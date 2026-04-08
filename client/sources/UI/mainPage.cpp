#include <QMessageBox>

#include "../UI/mainPage.h"

MainPage::MainPage(Controller* controller, QWidget *parent) : QWidget{parent}, m_controller{controller} {
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

        m_sync = new QPushButton("Sync");
        tabbar->addWidget(m_sync);

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

        setStatus(AppContext::SyncState::Unknown);
    }
}

void MainPage::setTableModel(TableModel* data) {
    qDebug() << "MainPage::setTableModel";
    if(data == nullptr)
        return;

    m_records = data;

    if (m_tableView && m_records)
        m_tableView->setModel(m_records);
}

void MainPage::setUserModel(const QStringList& data) {
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

        if(m_userNames->count() > 0) {
            m_userNames->blockSignals(true);
            m_userNames->setCurrentText(m_currentUserName);
            m_userNames->blockSignals(false);
        }
        setStatusUser();
    }
}

void MainPage::setStatus(AppContext::SyncState status) {
    qDebug() << "MainPage::setStatus";
    switch(status) {
    case SyncState::Unsynced:
        m_status->setText("🔴 Unsynced");
        m_status->setStyleSheet("color: red;");
        m_sync->setStyleSheet("color: red;");
        m_sync->setEnabled(true);
        break;
    case SyncState::Synced:
        m_status->setText("🟢 Synced");
        m_status->setStyleSheet("color: green;");
        m_sync->setStyleSheet("color: green;");
        m_sync->setEnabled(false);
        break;
    case SyncState::Unknown:
    default:
        m_status->setText("🟢 Unknown");
        m_status->setStyleSheet("color: gray;");
        m_sync->setStyleSheet("color: gray;");
        m_sync->setEnabled(false);
        break;
    }
}

void MainPage::setStatusUser() {
    qDebug() << "MainPage::onCurrentUserChanged";
    if(m_userNames->currentText() == m_currentUserName) {
        m_currentUser->setStyleSheet("color: green;");
    } else {
        m_currentUser->setStyleSheet("color: orange;");
    }
}

void MainPage::setupConnections() {
    qDebug() << "MainPage::setupConnections";

    //sync table
    connect(m_sync, &QPushButton::clicked,
            this, [this]() {
                bool responce = QMessageBox::question(
                                    this,
                                    "Syncronize",
                                    "Are you sure you want to synchronize?",
                                    QMessageBox::Yes | QMessageBox::No
                                    ) == QMessageBox::Yes;
                if(responce)
                    m_controller->onSync();
    });

    //rollback table
    connect(m_rollback, &QPushButton::clicked,
            this, [this]() {
                bool responce = QMessageBox::question(
                                    this,
                                    "Reset",
                                    "Are you sure you want to rollback?",
                                    QMessageBox::Yes | QMessageBox::No
                                    ) == QMessageBox::Yes;
                if(responce)
                    m_controller->onRollback();
    });

    //insert row
    connect(m_insert, &QPushButton::clicked,
            m_controller, &Controller::onInsertRow);

    //delete row
    connect(m_delete, &QPushButton::clicked,
            this, [this]() {
                            if (!m_tableView)
                                return;

                            QModelIndex index = m_tableView->currentIndex();
                            if (!index.isValid())
                                return;

                            m_controller->onDeleteRow(index.row());
                        });

    //change field
    connect(m_change, &QPushButton::clicked,
            this, [this]() {
                        if (!m_tableView)
                            return;

                        QModelIndex index = m_tableView->currentIndex();
                        if (!index.isValid())
                            return;

                            m_controller->onChangeField(index.row(), index.column(), index.data(Qt::DisplayRole));
                        });

    //update table
    connect(m_userNames, &QComboBox::currentTextChanged,
            this, [this] (const QString& text) {
                setStatusUser();
                m_controller->onUpdateTable(text);
                        });

    //from controller; set users model
    connect(m_controller, &Controller::usersChanged,
            this, [this](const QStringList& users) { setUserModel(users); });

    //from controller; set current user
    connect(m_controller, &Controller::currentUserChanged,
            this, [this](const QString& user) { setCurrentUser(user); });

    //from controller; set model
    connect(m_controller, &Controller::currentModelChanged,
            this, [this](TableModel* table) { setTableModel(table); });

    //from controller; set status
    connect(m_controller, &Controller::statusChanged,
            this, [this]() { setStatus(static_cast<SyncState>(m_controller->getStatus())); });
}
