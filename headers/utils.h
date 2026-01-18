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
        static inline QByteArray serialize(const uint32_t opcode, const QVariantMap& sendedPacket) {
            QByteArray payload; //for calculating size packet(with |- size -|- opcode -|- data -|)
            QDataStream payloadStream(&payload, QIODevice::WriteOnly);
            payloadStream.setVersion(QDataStream::Qt_6_0);

            for(auto it = sendedPacket.constBegin(); it != sendedPacket.constEnd(); ++it) {
                payloadStream << it.key() << it.value();
            }

            //result size packet
            uint32_t sizePacket = payload.size() + sizeof(sizePacket) + sizeof(opcode);
            QByteArray serializePacket;
            QDataStream out(&serializePacket, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_6_0);

            out << sizePacket << opcode;
            out.writeRawData(payload.constData(), payload.size());

            qDebug() << "Serialize::success";

            return serializePacket;
        }

        //static deserialize
        static inline std::pair<const uint32_t, QVariantMap> deserialize(const QByteArray& accepdedPacket/*checksum*/) {
            QVariantMap deserializePacket;
            QDataStream in(accepdedPacket);
            in.setVersion(QDataStream::Qt_6_0);

            uint32_t opcode;
            uint32_t size;  //size packet(with |- size -|- opcode -|- data -|)
            in >> size >> opcode; //size dont used, maybe will need later

            QByteArray payload = accepdedPacket.mid(sizeof(size) + sizeof(opcode));

            QDataStream payloadStream(payload);
            payloadStream.setVersion(QDataStream::Qt_6_0);

            QString key;
            QVariant value;
            while(!payloadStream.atEnd()) {
                payloadStream >> key >> value;
                if(payloadStream.status() != QDataStream::Ok)
                    break;
                deserializePacket.insert(key, value);
            }
            qDebug() << "Deserialize::success";

            return {opcode,deserializePacket};
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
