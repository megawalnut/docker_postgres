#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QVariantMap>

#include "dbhelper.h"

class ICommand {
public:
    virtual ~ICommand() = default;

    //method for a specific command
    virtual QVariant exec(QVariantMap&&) = 0;

    static void addDb(const QString& userName,
                       const QString& hostName,
                       const QString& dbName,
                       const QString& password) {
        if(!m_ptrDb) {
            m_ptrDb = std::make_unique<DBHelper>(userName, hostName, dbName, password);
            m_ptrDb->connect();
        } else {
            qDebug() << "DataBase is already exists";
        }
    }

private:
    static std::unique_ptr<DBHelper> m_ptrDb;
};

#endif // ICOMMAND_H
