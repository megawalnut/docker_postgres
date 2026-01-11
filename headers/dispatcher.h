#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QVariantMap>

#include "icommand.h"

class Dispatcher final {
private:
    explicit Dispatcher();
    ~Dispatcher();

    enum Opcode : uint32_t {
        None = 0,
        Login,
        Registry,
        Edit,
        Update,
        Get,
        Pull
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
};

#endif // DISPATCHER_H
