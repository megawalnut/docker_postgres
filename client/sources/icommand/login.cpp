#include "login.h"
#include "utils.h"
#include "serverOpcode.h"

Login::Login(DBHelper* db) : m_ptrDb(db) {}

QByteArray Login::exec(const QVariantMap& data) {
    QString username = data.value("name").toString();
    QString userPassword = data.value("password").toString();

    if(username.isEmpty() || userPassword.isEmpty()) {
        qDebug() << "Login::Ivalid data";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::login), {{"result", "FAILED"}});
    }

    //hashing userPassword
    QByteArray hashedPassword = QCryptographicHash::hash(userPassword.toUtf8(), QCryptographicHash::Sha256);
    QString hashedHex = QString(hashedPassword.toHex());

    //adding user in users table with userRoles
    auto [success, strings] = m_ptrDb->send("SELECT role FROM users WHERE username = ? AND password_hash = ?", {username, hashedHex});
    if(!success || strings.empty()) {
        qDebug() << "Login::failed";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::login), {{"result", "FAILED"}});
    }
    QString userRole = strings[0].value("role").toString();

    qDebug() << QString("Login::success for user %1 role:%2")
                        .arg(username)
                        .arg(userRole);
    return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::login), {{"result", "SUCCESS"}, {"userRole", userRole}});
}
