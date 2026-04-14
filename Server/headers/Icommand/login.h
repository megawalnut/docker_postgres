#ifndef LOGIN_H
#define LOGIN_H

#include <QVariantMap>

#include "icommand.h"

class Login : public ICommand {
public:
    Login() = default;
    ~Login() = default;

    //method for a login
    QVariantMap exec(const QVariantMap& data,
                     ClientConnection* m_client) override;
};

#endif // LOGIN_H
