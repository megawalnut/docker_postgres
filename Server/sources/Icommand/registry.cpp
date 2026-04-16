#include "registry.h"
#include "Utils/keys.h"
#include "Utils/protocol.h"

QVariantMap Registry::exec(const QVariantMap& data, ClientConnection* client) {
    ClientQueryStructure::Auth query;
    query.fromMap(data);

    ServerResponseStructure::Auth responce;
    responce.user.userName = query.user.userName;
    responce.result = ServerResponseStructure::Status::Failed;
    const auto& ptr_db = client->db();
    if(!ptr_db) {
        qWarning() << "Login::exec: Pointer to database is empy";
        return responce.toMap();
    }

    if (query.user.userName.isEmpty() || query.userPass.isEmpty()) {
        qWarning() << "Registry::exec: Invalid data";
        return responce.toMap();
    }

    //------------------- INSERT --------------------
    QString sql = "INSERT INTO users (user_name, password_hash) VALUES (?, ?) RETURNING id";
    auto [success, res] = ptr_db->send(sql, {query.user.userName, query.userPass});
    if (!success || res.empty()) {
        qWarning() << "Registry::exec: Failed INSERT INTO to database";
        return responce.toMap();
    }

    int userId = res[0].value(Keys::UserInfo_UserId).toInt();

    QString tableName = "user_" + QString::number(userId, 10);

    //------------------- UPDATE --------------------
    sql = "UPDATE users SET table_name=? WHERE id=?";
    std::tie(success, std::ignore) = ptr_db->send(sql, {tableName, userId});
    if (!success) {
        qWarning() << "Registry::exec: Failed UPDATE users";
        return responce.toMap();
    }

    //------------------- CREATE --------------------
    sql = QString("CREATE TABLE %1 (id SERIAL PRIMARY KEY, type TEXT, name TEXT, brand TEXT, model TEXT, year INT)").arg(tableName);
    std::tie(success, std::ignore) = ptr_db->send(sql, {});
    if (!success) {
        qWarning() << "Registry::exec: Failed CREATE TABLE " << tableName;
        return responce.toMap();
    }

    responce.user.userId = userId;
    responce.user.tableName = tableName;

    responce.result = ServerResponseStructure::Status::Success;

    client->setUser(responce.user);

    qDebug() << "Registry success " << query.user.userName;

    return responce.toMap();
}
