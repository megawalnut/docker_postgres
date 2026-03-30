#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QVariantMap>
#include <QIODevice>
#include <QDataStream>

//namespace for additional functions
namespace Utils {
    // class - structure of packet
    // structure packet
    // |- size -|- opcode -|- data -|
    class Packet final {
    public:
        //static serialize
        Packet() = delete;

        static QByteArray serialize(uint32_t opcode, const QVariantMap& packet) {
            QByteArray serializePacket; //for calculating size packet(with |- size -|- opcode -|- data -|)
            QDataStream out(&serializePacket, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);

            uint32_t size = 0;
            out << size << opcode << packet;

            size = serializePacket.size();

            // fix size
            QDataStream fix(&serializePacket, QIODevice::WriteOnly);
            fix.setVersion(QDataStream::Qt_6_0);

            //rewriting start 'serializePacket'!!
            fix << size;

            return serializePacket;
        }

        //static deserialize
        static std::pair<uint32_t, QVariantMap>deserialize(const QByteArray& accepdedPacket/*checksum*/) {
            QVariantMap deserializePacket;
            QDataStream in(accepdedPacket);
            in.setVersion(QDataStream::Qt_6_0);

            uint32_t opcode;
            uint32_t size;  //size packet(with |- size -|- opcode -|- data -|)

            in >> size >> opcode >> deserializePacket; //size dont used, maybe will need later

            qDebug() << "Deserialize::success";
            return {opcode, deserializePacket};
        }

        static inline uint32_t getSize(const QByteArray& accepdedPacket) {
            uint32_t size;
            QDataStream in(accepdedPacket);
            in.setVersion(QDataStream::Qt_6_0);
            if(in.status() != QDataStream::Ok)
                return 0;
            in >> size;
            return size;
        }
    };
}
#endif // PACKET_H
