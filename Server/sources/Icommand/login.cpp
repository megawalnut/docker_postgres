#include "login.h"
#include "Utils/keys.h"
#include "Utils/protocol.h"

QVariantMap Login::exec(const QVariantMap& data, ClientConnection* client) {
    ClientQueryStructure::Auth query;
    query.fromMap(data);

    //create responce
    ServerResponseStructure::Auth responce;
    responce.user.userName = query.user.userName;
    responce.result = ServerResponseStructure::Status::Failed;

    const auto& ptr_db = client->db();
    if(!ptr_db) {
        qWarning() << "Login::exec: Pointer to database is empy";
        return responce.toMap();
    }

    if (query.user.userName.isEmpty() || query.userPass.isEmpty()) {
        qWarning() << "Login::exec: Invalid data";
        return responce.toMap();
    }

    //------------------- SELECT --------------------
    QString sql = "SELECT id, table_name FROM users WHERE user_name=? AND password_hash=?";
    auto [success, res] = ptr_db->send(sql, {query.user.userName, query.userPass});
    if (!success || res.empty()) {
        qWarning() << "Login::exec: Failed SELECT users";
        return responce.toMap();
    }

    const QSqlRecord& row = res[0];

    //adding responce
    responce.user.userId = row.value(Keys::UserInfo_UserId).toInt();
    responce.user.tableName = row.value(Keys::UserInfo_TableName).toString();

    responce.result = ServerResponseStructure::Status::Success;

    client->setUser(responce.user);

    qDebug() << "Login::exec: Success " << query.user.userName;

    return responce.toMap();
}
