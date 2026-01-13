#include "clientConnection.h"
#include "utils.h"
#include "dispatcher.h"

ClientConnection::ClientConnection(QTcpSocket* const socket) : m_clientSocket(socket) {}
ClientConnection::~ClientConnection() {}

void ClientConnection::onDisconnected() {
    emit disconnected();
}

void ClientConnection::onReadyRead() {
    m_buffer.append(m_clientSocket->readAll());

    while(m_buffer.size() >= sizeof(uint32_t)) {
        const uint32_t realPackageSize = Utils::Packet::getSize(m_buffer);
        if(m_buffer.size() < realPackageSize)
            break; //wait full packet

        qDebug() << "The package came from a " << m_clientSocket->peerAddress() << ":" << m_clientSocket->peerPort();
        QByteArray m_acceptedPackage = m_buffer.left(realPackageSize);   //package data
        m_buffer.remove(0, realPackageSize);
        std::pair<const uint32_t, QVariantMap> data = Utils::Packet::deserialize(m_acceptedPackage);
        Dispatcher::instance()->dispatch(data.first, std::move(data.second));
    }
}
