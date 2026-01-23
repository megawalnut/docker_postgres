#include "registry.h"
#include "utils.h"
#include "dispatcher.h"

Registry::Registry() {}
Registry::~Registry() {}

QByteArray Registry::exec(QVariantMap&& data) {
    QString role = data.value("role").toString();
    QString username = data.value("name").toString();
    QString userPassword = data.value("password").toString();
    QString userRole = data.value("userRole").toString();

    if(username.isEmpty() || userPassword.isEmpty()) {
        qDebug() << "Registry::Ivalid data";
        return Utils::Packet::serialize(Dispatcher::Opcode::registry_, {{"result", "FAILED"}});
    }
    if(role != "superuser" || userRole == "superuser") {
        qDebug() << "Registry::Access denied";
        return Utils::Packet::serialize(Dispatcher::Opcode::registry_, {{"result", "FAILED"}});
    }

    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(userPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashedHex = QString(hashedPassword.toHex());

    //adding user in users table with userRoles
    auto [success, strings] = m_ptrDb->send(QString("INSERT INTO users (username, password_hash, role)"
                                                    "VALUES ('%1', '%2', '%3')")
                                                    .arg(username)
                                                    .arg(hashedHex)
                                                    .arg(userRole));
    if(!success) {
        qDebug() << "Registry::failed";
        return Utils::Packet::serialize(Dispatcher::Opcode::registry_, {{"result", "FAILED"}});
    }
    qDebug() << "Registry::success";
    return Utils::Packet::serialize(Dispatcher::Opcode::registry_, {{"result", "SUCCESS"}});
}
