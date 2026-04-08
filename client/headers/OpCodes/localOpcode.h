#ifndef LOCALOPCODE_H
#define LOCALOPCODE_H

#include <cstdint>

enum class LocalOpcode : uint32_t {
    Unknown = 0,
    Insert,
    Delete,
    Change,
    Update
};
#endif //LOCALOPCODE_H
