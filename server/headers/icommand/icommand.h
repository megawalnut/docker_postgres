#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QVariantMap>

#include "dbhelper.h"

class ICommand {
public:
    virtual ~ICommand() = default;

    //method for a specific command
    virtual QByteArray exec(QVariantMap&&) = 0;

    static void addDb(std::unique_ptr<DBHelper>&& ptr/*std::move*/) {
        if(!m_ptrDb) {
            m_ptrDb = std::move(ptr);
        } else {
            qDebug() << "DataBase is already exists";
        }
    }

protected:
    static inline std::unique_ptr<DBHelper> m_ptrDb = nullptr;
};

#endif // ICOMMAND_H
