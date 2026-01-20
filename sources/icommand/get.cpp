#include "get.h"
#include "utils.h"
#include "dispatcher.h"


Get::Get() {}
Get::~Get() {}

QByteArray Get::exec(QVariantMap&& data) {
    Q_UNUSED(data);

    auto [ok, tables] = m_ptrDb->send("SELECT table_name FROM information_schema.tables "
                                      "WHERE table_schema='public' AND table_type='BASE TABLE';");

    QVariantMap result{{"result", "SUCCESS"}};
    if(ok) {
        for(const auto& table : tables) {
            auto [success, strings] = m_ptrDb->send(QString("SELECT * FROM %1")
                                                        .arg(table.value(0).toString()));
            if(!success) {
                qDebug() << "GET::failed getdata from dataBase";
                return Utils::Packet::serialize(Dispatcher::Opcode::get_, {{"result", "FAILED"}});
            }

            QVariantList linesInTable;
            for (const auto& str : strings) {
                QVariantMap oneLine;
                for (int i = 0; i < str.count(); ++i) {
                    //This is where the row entry in the table is collected
                    oneLine.insert(str.fieldName(i), str.value(i));
                }
                //Here, a dictionary of record strings is collected in one table
                linesInTable.push_back(QVariant(oneLine));
            }
            //A collection of tables is being collected here
            result.insert(table.fieldName(0), linesInTable);
        }
        qDebug() << "GET::success";
        return Utils::Packet::serialize(Dispatcher::Opcode::get_, result);
    }
    qDebug() << "GET::failed";
    return {};
}

