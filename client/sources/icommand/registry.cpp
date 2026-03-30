#include "registry.h"
#include "utils.h"
#include "serverOpcode.h"

Registry::Registry(DBHelper* db) : m_ptrDb(db) {}

QByteArray Registry::exec(const QVariantMap& data) {
    QString role = data.value("role").toString();
    QString username = data.value("name").toString();
    QString userPassword = data.value("password").toString();
    QString userRole = data.value("userRole").toString();

    if(username.isEmpty() || userPassword.isEmpty()) {
        qDebug() << "Registry::Ivalid data";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::registry), {{"result", "FAILED"}});
    }
    if(role != "superuser") {
        qDebug() << "Registry::Access denied";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::registry), {{"result", "FAILED"}});
    }

    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(userPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashedHex = QString(hashedPassword.toHex());

    //adding user in users table with userRoles
    auto [success, strings] = m_ptrDb->send("INSERT INTO users (username, password_hash, role) VALUES (?, ?, ?)",
                                            {username, hashedHex, userRole});
    if(!success) {
        qDebug() << "Registry::failed";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::registry), {{"result", "FAILED"}});
    }
    qDebug() << "Registry::success";
    return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::registry), {{"result", "SUCCESS"}});
}
