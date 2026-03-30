#include "get.h"
#include "utils.h"
#include "serverOpcode.h"

Get::Get(DBHelper* db) : m_ptrDb(db) {}

QByteArray Get::exec(const QVariantMap& data) {
    Q_UNUSED(data);

    auto [ok, tables] = m_ptrDb->send("SELECT table_name FROM information_schema.tables "
                                      "WHERE table_schema='public' AND table_type='BASE TABLE';", {});

    if (!ok) {
        qDebug() << "GET::failed (cannot fetch tables)";
        return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::get), {{"result", "FAILED"}});
    }

    QVariantMap result;
    result.insert("status", "SUCCESS");

    for (const auto& table : tables) {

        QString tableName = table.value(0).toString();
        auto [success, rows] = m_ptrDb->send(QString("SELECT * FROM %1").arg(tableName), {});

        if (!success) {
            qDebug() << "GET::failed getdata from table" << tableName;
            return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::get), {{"result", "FAILED"}});
        }

        QVariantList linesInTable;

        for (const auto& row : rows) {
            QVariantMap oneLine;

            for (int i = 0; i < row.count(); ++i) {
                oneLine.insert(row.fieldName(i), row.value(i));
            }

            linesInTable.push_back(oneLine);
        }

        result.insert(tableName, linesInTable);
    }

    qDebug() << "GET::success";
    return Utils::Packet::serialize(static_cast<uint32_t>(ServerOpcode::get), result);
}
