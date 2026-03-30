#include "update.h"
#include "utils.h"
#include "serverOpcode.h"

Update::Update(DBHelper* db) : m_ptrDb(db) {}

QByteArray Update::exec(const QVariantMap& data) {
    for (const auto& tableName : data.keys()) {
        QVariantList rows = data.value(tableName).toList();
        for (const auto& field : rows) {
            QVariantMap mapData = field.toMap();
            int id = mapData.value("id").toInt();
            for (auto it = mapData.begin(); it != mapData.end(); ++it) {
                if (it.key() == "id")
                    continue;

                QString queryStr = QString("UPDATE %1 SET %2 = ? WHERE id = ?")
                                       .arg(tableName)
                                       .arg(it.key());

                auto [success, _] =m_ptrDb->send(queryStr,{it.value().toString(), id});

                if (!success) {
                    qDebug() << "Update::failed";
                    return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::update), {{"result", "FAILED"}});
                }
            }
        }
    }

    qDebug() << "Update::success";
    return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::update), {{"result", "SUCCESS"}});
}
