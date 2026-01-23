#include "dispatcher.h"
#include "registry.h"
#include "login.h"
#include "get.h"
#include "update.h"

Dispatcher* Dispatcher::m_singleDispatcher = nullptr;

Dispatcher::Dispatcher() {
    m_ptrCommands.push_back(std::make_unique<Login>());
    m_ptrCommands.push_back(std::make_unique<Registry>());
    m_ptrCommands.push_back(std::make_unique<Update>());
    m_ptrCommands.push_back(std::make_unique<Get>());
    qDebug() << "Dispatcher::The dispetcher was created";
}
Dispatcher::~Dispatcher() {}

QByteArray Dispatcher::dispatch(const uint32_t command, QVariantMap&& data/*std::move*/) {
    QMutexLocker locker(&m_mutex);

    if(command >= m_ptrCommands.size()) {
        qDebug() <<  QString("Dispatcher::invalid command index %1")
                             .arg(command);
        return {};
    }

    qDebug() <<  QString("Dispatcher::called %1")
                         .arg(static_cast<Opcode>(command));
    return m_ptrCommands[command]->exec(std::move(data));
}
