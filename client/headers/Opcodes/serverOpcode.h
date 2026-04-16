#ifndef SERVEROPCODE_H
#define SERVEROPCODE_H

#include <cstdint>

enum class ServerOpcode : uint32_t {
    Login = 0,
    Registry,
    Sync,
    Rollback,
    FullDump,
    Unknown
};
#endif //SERVEROPCODE_H
