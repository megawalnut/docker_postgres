#ifndef SERVEROPCODE_H
#define SERVEROPCODE_H

#include <cstdint>

enum class ServerOpcode : uint32_t{
    unknown = 0,
    login,
    registry,
    update,
    get
};
#endif //SERVEROPCODE_H

