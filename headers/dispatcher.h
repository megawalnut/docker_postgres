#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <cstdint>

class Dispatcher final {
private:
    explicit Dispatcher();
    ~Dispatcher();

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

public:
    static inline Dispatcher* instance() {
        if(!m_singleDispatcher)
            m_singleDispatcher = new Dispatcher();
        return m_singleDispatcher;
    }

    void dispatch(uint32_t command);

private:
    static Dispatcher* m_singleDispatcher;
};

#endif // DISPATCHER_H
