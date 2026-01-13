#ifndef LOGIN_H
#define LOGIN_H

#include <QVariantMap>

#include "icommand.h"

class Login : public ICommand {
public:
    Login();
    ~Login() override;

    //method for a login
    QVariant exec(QVariantMap&&) override;
};

#endif // LOGIN_H
