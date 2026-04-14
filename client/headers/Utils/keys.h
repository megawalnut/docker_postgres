#ifndef KEYS_H
#define KEYS_H

#include <QString>

namespace Keys {
    //---------------------- UserInfo -------------------------
    inline const QString UserInfo_UserId           = "id";
    inline const QString UserInfo_UserName         = "user_name";
    inline const QString UserInfo_TableName        = "table_name";
    inline const QString UserInfo_UserInfo         = "user_info";

    //-------------------- ServerResponse ---------------------
    inline const QString ServerResponse_Result     = "result";
    inline const QString ServerResponse_Tables     = "tables";
    inline const QString ServerResponse_Snapshot   = "snapshot";
    inline const QString ServerResponse_Users      = "users";
    inline const QString ServerResponse_Columns    = "columns";
    inline const QString ServerResponse_Rows       = "rows";
    inline const QString ServerResponse_Id         = "id";
    inline const QString ServerResponse_Value      = "value";

    //--------------------- ClientQuery -----------------------
    inline const QString ClientQuery_UserPass      = "password_hash";

    //------------------- PacketStructure ---------------------
    inline const QString PacketStructure_Columns   = "columns";
    inline const QString PacketStructure_Values    = "values";
    inline const QString PacketStructure_RowId     = "id";
    inline const QString PacketStructure_Rows      = "rows";

    //---------------------- Operation ------------------------
    inline const QString Operation_Changes         = "changes";
    inline const QString Operation_Operation       = "operation";
    inline const QString Operation_Data            = "data";
}

#endif // KEYS_H
