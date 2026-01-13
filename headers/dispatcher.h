#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QVariantMap>
#include <QMutex>

#include "icommand.h"

class Dispatcher final {
private:
    explicit Dispatcher();
    ~Dispatcher();

    enum Opcode : uint32_t {
        none_ = 0,
        login_,
        registry_,
        edit_,
        update_,
        get_,
        pull_
    };

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

public:
    static inline Dispatcher* instance() {
        if(!m_singleDispatcher)
            m_singleDispatcher = new Dispatcher();
        return m_singleDispatcher;
    }

    void dispatch(const uint32_t command, QVariantMap&& data);

private:
    static Dispatcher* m_singleDispatcher;
    std::vector<std::unique_ptr<ICommand>> m_ptrCommands;
    QMutex m_mutex;
};

#endif // DISPATCHER_H
