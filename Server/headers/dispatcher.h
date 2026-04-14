#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QVariantMap>
#include <QMutex>

#include "icommand.h"
#include "../Opcodes/serverOpcode.h"

using ServerOpcode = ServerOpcode;

class Dispatcher final {
public:
    static Dispatcher& instance() {
        qDebug() << "Dispatcher::instance";
        static Dispatcher dispatcher;
        return dispatcher;
    }

    QVariantMap dispatch(const uint32_t command,
                         const QVariantMap& data,
                         ClientConnection* client);

private:
    explicit Dispatcher();
    ~Dispatcher() = default;

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

    std::vector<std::unique_ptr<ICommand>> m_listCommands;
    std::vector<QString> commandsToString = { "Login", "Registry", "Sync", "Rollback", "", "FullDump"};
    QMutex m_mutex;
};

#endif // DISPATCHER_H
