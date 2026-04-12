#include "clientConnection.h"
#include "utils.h"
#include "dispatcher.h"

ClientConnection::ClientConnection(QTcpSocket* const socket) : m_clientSocket(socket) {
    m_clientSocket = socket;
    m_clientSocket->setParent(this);
}

void ClientConnection::onDisconnected() {
    emit disconnected();
}

void ClientConnection::onReadyRead() {
    //read from socket and save
    m_buffer.append(m_clientSocket->readAll());

    //if received a part of the package < 4b
    while(m_buffer.size() >= MIN_PACKET_PART) {
        uint32_t realPackageSize;
        QDataStream stream(&m_buffer, QIODevice::ReadOnly);

        stream.setVersion(QDataStream::Qt_6_0);
        stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);

        // first 4 bytes - real package size(See also: utiils::structure_packet)
        stream >> realPackageSize;

        if (realPackageSize > MAX_PACKET_SIZE) {
            qWarning() << "Client::onReadyRead: Too big packet";
            m_buffer.clear();
            return;
        }

        //if received a part of the package
        if(m_buffer.size() < realPackageSize) {
            break; //wait full packet
        }

        if (realPackageSize < MIN_PACKET_SIZE) {
            qWarning() << "Client::onReadyRead: Invalid packet";
            m_buffer.clear();
            return;
        }

        qDebug() << QString("Client::onReadyRead: The package came from a %1:%2")
                        .arg(m_serverAddress)
                        .arg(m_portNum);


        QByteArray acceptedPackage = m_buffer.left(realPackageSize);   //package data
        m_buffer.remove(0, realPackageSize);

        onPacketReady(acceptedPackage);
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
