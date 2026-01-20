#include "update.h"
#include "utils.h"
#include "dispatcher.h"

Update::Update() {}
Update::~Update() {}

QByteArray Update::exec(QVariantMap&& data) {
    for(const auto tableName : data.keys()) {
        QVariantList rows = data.value(tableName).toList();
        for(const auto field : rows) {
            QVariantMap mapData = field.toMap();
            for(const auto record : mapData.keys()) {
                auto [success, strings] = m_ptrDb->send(QString("UPDATE %1 SET %2 WHERE id=%3")
                                                                .arg(tableName)
                                                                .arg(mapData.value(record).toString())
                                                                .arg(record));
                if(!success) {
                    qDebug() << "Update::failed";
                    return Utils::Packet::serialize(Dispatcher::Opcode::update_, {{"result", "FAILED"}});
                }
            }
        }
    }
    qDebug() << "Update::success";
    return Utils::Packet::serialize(Dispatcher::Opcode::update_, {{"result", "SUCCESS"}});
}
