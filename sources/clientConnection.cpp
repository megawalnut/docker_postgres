#include "clientConnection.h"
#include "utils.h"
#include "dispatcher.h"

ClientConnection::ClientConnection(QTcpSocket* const socket) : m_clientSocket(socket) {
    connect(this, &ClientConnection::packetReady,
            this, &ClientConnection::onPacketReady);
}
ClientConnection::~ClientConnection() {}

void ClientConnection::onDisconnected() {
    emit disconnected();
}

void ClientConnection::onReadyRead() {
    //read from socket and save
    m_buffer.append(m_clientSocket->readAll());

    //if received a part of the package < 4b
    while(m_buffer.size() >= sizeof(uint32_t)) {
        const uint32_t realPackageSize = Utils::Packet::getSize(m_buffer);
        //if received a part of the package
        if(m_buffer.size() < realPackageSize)
            break; //wait full packet

        qDebug() << "ClientConnection::success";
        qDebug() << QString("The package came from a %1 : %2")
                            .arg(m_clientSocket->peerAddress().toString())
                            .arg(m_clientSocket->peerPort());
        QByteArray m_acceptedPackage = m_buffer.left(realPackageSize);   //package data
        m_buffer.remove(0, realPackageSize);
        std::pair<const uint32_t, QVariantMap> data = Utils::Packet::deserialize(m_acceptedPackage);
        QByteArray receivedPacket = Dispatcher::instance()->dispatch(data.first, std::move(data.second));
        emit packetReady(receivedPacket);
    }
}

void ClientConnection::onPacketReady(const QByteArray& receivedPacket) {
    if(!m_clientSocket || m_clientSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "ClientConnection::failed!Socket not connected";
        return;
    }
    if(receivedPacket.isEmpty()) {
        qDebug() << "ClientConnection::Invalid received data";
        return;
    }

    qint64 bytesWritten = m_clientSocket->write(receivedPacket);
    if(bytesWritten == -1) {
        qDebug() << "ClientConnection::failed!Write error";
        return;
    }

    bool flushed = m_clientSocket->flush();
    if(flushed)
        qDebug() << "ClientConnection::success!Writed";
    else
    qDebug() << "ClientConnection::failed!Flush error";
}
