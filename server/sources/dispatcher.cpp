#include "dispatcher.h"
#include "registry.h"
#include "login.h"
#include "get.h"
#include "update.h"

Dispatcher* Dispatcher::m_singleDispatcher = nullptr;

Dispatcher::Dispatcher() {
    m_listCommands.push_back(std::make_unique<Login>());
    m_listCommands.push_back(std::make_unique<Registry>());
    m_listCommands.push_back(std::make_unique<Sync>());
    m_listCommands.push_back(std::make_unique<Rollback>());
    m_listCommands.push_back(std::make_unique<FullDump>());
    qDebug() << "Dispatcher::The dispetcher was created";
}

QByteArray Dispatcher::dispatch(const uint32_t command, QVariantMap&& data/*std::move*/) {
    QMutexLocker locker(&m_mutex);

    if(command >= m_listCommands.size()) {
        qDebug() <<  QString("Dispatcher::invalid command index %1")
                             .arg(command);
        return {};
    }

    qDebug() <<  QString("Dispatcher::called %1")
                         .arg(static_cast<Opcode>(command));
    return m_listCommands[command]->exec(std::move(data));
}
