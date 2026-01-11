#include "dispatcher.h"
#include "registry.h"
#include "login.h"
#include "get.h"
#include "pull.h"
#include "update.h"
#include "edit.h"

Dispatcher* Dispatcher::m_singleDispatcher = nullptr;

Dispatcher::Dispatcher() {
    m_ptrCommands.push_back(std::make_unique<Login>());
    m_ptrCommands.push_back(std::make_unique<Registry>());
    m_ptrCommands.push_back(std::make_unique<Edit>());
    m_ptrCommands.push_back(std::make_unique<Get>());
    m_ptrCommands.push_back(std::make_unique<Pull>());
    m_ptrCommands.push_back(std::make_unique<Update>());
}
Dispatcher::~Dispatcher() {}

void Dispatcher::dispatch(const uint32_t command, QVariantMap&& data/*std::move*/) {
    if(command >= m_ptrCommands.size()) {
        qDebug() <<  "Dispatcher: invalid command index" << command;
        return;
    }
    m_ptrCommands[command]->exec(std::move(data));
    return;
}
