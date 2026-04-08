#ifndef SERVEROPCODE_H
#define SERVEROPCODE_H

#include <cstdint>

enum class ServerOpcode : uint32_t {
    Unknown = 0,
    Users,
    Login,
    Registry,
    Sync,
    Rollback,
    FullDump
};
#endif //SERVEROPCODE_H
