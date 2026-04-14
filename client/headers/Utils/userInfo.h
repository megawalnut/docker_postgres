#ifndef USERINFO_H
#define USERINFO_H

#include <QVariantMap>

#include "keys.h"

struct UserInfo {
    int userId = -1;
    QString userName;
    QString tableName;

    void fromMap(const QVariantMap& map) {
        userId = map.value(Keys::UserInfo_UserId).toInt();
        userName = map.value(Keys::UserInfo_UserName).toString();
        tableName = map.value(Keys::UserInfo_TableName).toString();
    }

    QVariantMap toMap() const {
        return {
            {Keys::UserInfo_UserId, userId},
            {Keys::UserInfo_UserName, userName},
            {Keys::UserInfo_TableName, tableName}
        };
    }
};

#endif // USERINFO_H
