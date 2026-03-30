#ifndef LOGIN_H
#define LOGIN_H

#include <QVariantMap>

#include "icommand.h"
#include "dbhelper.h"

class Login : public ICommand {
public:
    explicit Login(DBHelper* db);

    //method for a login
    QByteArray exec(const QVariantMap& data) override;

private:
    DBHelper* m_ptrDb;
};

#endif // LOGIN_H
