#include "login.h"
#include "utils.h"
#include "dispatcher.h"

Login::Login() {}
Login::~Login() {}

QByteArray Login::exec(QVariantMap&& data) {
    QString username = data.value("name").toString();
    QString userPassword = data.value("password").toString();

    if(username.isEmpty() || userPassword.isEmpty()) {
        qDebug() << "Login::Ivalid data";
        return Utils::Packet::serialize(Dispatcher::Opcode::login_, {{"result", "FAILED"}});
    }

    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(userPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashedHex = QString(hashedPassword.toHex());

    //adding user in users table with userRoles
    auto [success, strings] = m_ptrDb->send(QString("SELECT role FROM users WHERE username='%1' AND password_hash='%2'")
                                                    .arg(username)
                                                    .arg(hashedHex));
    if(!success || strings.empty()) {
        qDebug() << "Login::failed";
        return Utils::Packet::serialize(Dispatcher::Opcode::login_, {{"result", "FAILED"}});
    }
    QString userRole = strings[0].value("role").toString();

    qDebug() << QString("Login::success for user %1 role:%2")
                        .arg(username)
                        .arg(userRole);
    return Utils::Packet::serialize(Dispatcher::Opcode::login_, {{"result", "SUCCESS"}, {"userRole", userRole}});
}
