#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QString>
#include "DBHelpers/dbhelper.h"

extern const QString PATH;

class AppContext final {
public:
    struct UserInfo {
        int userId = -1;
        QString userName;
        QSet<QString> tables;
    };

public:
    void init(int userId, const QString& name, const QSet<QString>& tables, const QString& dbPath = PATH) {
        qDebug() << "AppContext::init";
        currentUser = {userId, name, tables};
        if (!m_db) {
            qWarning() << "AppContext::init: m_db is nullptr";
            m_db = std::make_unique<DBHelper>(dbPath);
        }
    }

    static AppContext& instance() {
        qDebug() << "AppContext::instance";
        static AppContext appContext;
        return appContext;
    }

    DBHelper& db() {
        qDebug() << "AppContext::db";
        Q_ASSERT(m_db);
        return *m_db;
    }

public:
    QList<UserInfo> users;
    UserInfo currentUser;

private:
    explicit AppContext() = default;
    ~AppContext() = default;

    explicit AppContext(const AppContext& other) = delete;
    AppContext& operator =(const AppContext& other) = delete;

    std::unique_ptr<DBHelper> m_db = nullptr;
};

#endif // APPCONTEXT_H
