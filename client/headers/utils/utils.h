#ifndef PACKET_H
#define PACKET_H

#include <QByteArray>
#include <QVariantMap>
#include <QIODevice>
#include <QDataStream>

#include "../OpCodes/serverOpcode.h"

constexpr uint32_t MAX_PACKET_SIZE = 10 * 1024 * 1024;
constexpr uint32_t MIN_PACKET_SIZE = sizeof(uint32_t) * 2;  //just minimal packetSize
constexpr uint32_t MIN_PACKET_PART = sizeof(uint32_t);  //minimal size(bytes) for read 'packetSize'


//namespace for additional functions
namespace Utils {
    // class - structure of packet
    // structure packet
    // |- size -|- opcode -|- data -|
    class Packet final {
    public:
        //static serialize
        Packet() = delete;
        static inline QByteArray serialize(uint32_t opcode, const QVariantMap& sendedPacket) {
            qDebug() << "Utils::serialize started";
            QByteArray payload; //for calculating size packet(with |- size -|- opcode -|- data -|)

            QDataStream payloadStream(&payload, QIODevice::WriteOnly);
            payloadStream.setVersion(QDataStream::Qt_6_0);
            payloadStream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

            for(auto it = sendedPacket.constBegin(); it != sendedPacket.constEnd(); ++it) {
                payloadStream << it.key() << it.value();
            }

            //result size packet
            uint32_t sizePacket = payload.size() + MIN_PACKET_SIZE;

            QByteArray serializePacket;

            QDataStream out(&serializePacket, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);
            out.setByteOrder(QDataStream::ByteOrder::LittleEndian);

            out << sizePacket << opcode;
            out.writeRawData(payload.constData(), payload.size());

            qDebug() << "Serialize::success";

            return serializePacket;
        }

        //static deserialize
        static inline std::pair<ServerOpcode, QVariantMap> deserialize(const QByteArray& accepdedPacket/*checksum*/) {
            qDebug() << "Utils::deserialize started";
            QVariantMap deserializePacket;

            QDataStream in(accepdedPacket);
            in.setVersion(QDataStream::Qt_6_0);
            in.setByteOrder(QDataStream::ByteOrder::LittleEndian);

            uint32_t opcode;
            uint32_t size;  //size packet(with |- size -|- opcode -|- data -|)

            in >> size >> opcode; //size dont used, maybe will need later

            if (size != accepdedPacket.size()) {
                qWarning() << "Packet size mismatch!";
                return {ServerOpcode::Unknown, {}};
            }

            if (size > MAX_PACKET_SIZE) {
                qWarning() << "Packet too big!";
                return {ServerOpcode::Unknown, {}};
            }

            QByteArray payload = accepdedPacket.mid(MIN_PACKET_SIZE);  // size + opcode

            QDataStream payloadStream(payload);
            payloadStream.setVersion(QDataStream::Qt_6_0);
            payloadStream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

            QString key;
            QVariant value;
            while(!payloadStream.atEnd()) {
                payloadStream >> key >> value;
                if (payloadStream.status() != QDataStream::Ok) {
                    qWarning() << "Payload stream error";
                    return {ServerOpcode::Unknown, {}};
                }
                deserializePacket.insert(key, value);
            }
            qDebug() << "Deserialize::success";

            return {static_cast<ServerOpcode>(opcode),deserializePacket};
        }
    };
}
#endif // PACKET_H
