#ifndef LOCALOPCODE_H
#define LOCALOPCODE_H

#include <cstdint>

enum class LocalOpcode : uint32_t{
    Unknown = 0,
    Insert,
    Delete,
    Update,
    Select
};
#endif //LOCALOPCODE_H
