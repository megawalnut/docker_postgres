#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QVariantMap>
#include <QMutex>

#include "icommand.h"
#include "login.h"
#include "registry.h"
#include "sync.h"
#include "rollback.h"
#include "fulldump.h"

class Dispatcher final {
private:
    explicit Dispatcher();
    ~Dispatcher() = default;

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

public:
    enum class Opcode : uint32_t {
        Unknown = 0,
        Login,
        Registry,
        Sync,
        Rollback,
        FullDump
    };

    static inline Dispatcher* instance() {
        if(!m_singleDispatcher)
            m_singleDispatcher = new Dispatcher();
        return m_singleDispatcher;
    }

    QByteArray dispatch(const uint32_t command, QVariantMap&& data);

private:
    static Dispatcher* m_singleDispatcher;
    std::vector<std::unique_ptr<ICommand>> m_listCommands;
    QMutex m_mutex;
};

#endif // DISPATCHER_H
