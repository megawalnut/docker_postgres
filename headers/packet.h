#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>

class Packet final {
public:
    //static serialize
    static inline QByteArray serialization(uint32_t command, const QByteArray& data) {
    }

    //static desialize
    static inline std::pair<uint32_t, QByteArray> deserialization() {
    }
};

#endif // PACKET_H
