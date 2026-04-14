#include "dispatcher.h"
#include "login.h"
#include "registry.h"
#include "sync.h"
#include "rollback.h"
#include "fulldump.h"

Dispatcher::Dispatcher() {
    m_listCommands.push_back(std::make_unique<Login>());
    m_listCommands.push_back(std::make_unique<Registry>());
    m_listCommands.push_back(std::make_unique<Sync>());
    m_listCommands.push_back(std::make_unique<Rollback>());
    m_listCommands.push_back(std::make_unique<FullDump>());
    qDebug() << "Dispatcher::Dispatcher: The dispetcher was created";
}

QVariantMap Dispatcher::dispatch(const uint32_t command,
                                 const QVariantMap& data,
                                 ClientConnection* client) {
    QMutexLocker locker(&m_mutex);

    if(command >= m_listCommands.size() ||
        command >= commandsToString.size())
    {
        qWarning() <<  QString("Dispatcher::dispatch: Invalid command index %1").arg(command);
        return {};
    }

    qDebug() <<  QString("Dispatcher::dispatch: Called %1")
                    .arg(commandsToString[command]);

    const auto& responce = m_listCommands[command]->exec(data, client);
    return responce;
}
