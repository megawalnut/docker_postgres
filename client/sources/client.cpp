#include "client.h"
#include "utils.h"

Client::Client(const quint16 portNum,
               const QString& serverAddress,
               QObject* parent)
    :
    QObject(parent),
    m_portNum(portNum),
    m_serverAddress(serverAddress),
    m_reconnecting(false)
{
    qDebug() << "Client::client: Client created";

    m_serverSocket = new QTcpSocket(this);

    qDebug() << "Client::client: Connection...";

    m_serverSocket->connectToHost(m_serverAddress, m_portNum);

    connect(m_serverSocket, &QTcpSocket::readyRead,
            this, &Client::onReadyRead);

    connect(m_serverSocket, &QTcpSocket::connected,
            this, &Client::onConnected);

    connect(m_serverSocket, &QTcpSocket::disconnected,
            this, &Client::onDisconnected);

    connect(m_serverSocket, &QTcpSocket::errorOccurred,
            this, &Client::onError);
}

void Client::onConnected() {
    qDebug() << QString("Client::onConnected: Connected to server on address %1:%2")
                    .arg(m_serverAddress)
                    .arg(m_portNum);

    m_reconnecting = false;
}

void Client::onReadyRead() {
    //read from socket and save
    m_buffer.append(m_serverSocket->readAll());

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

        emit packetReady(acceptedPackage);
    }
}

void Client::sendPacket(const QByteArray& clientPacket) {
    if(!connected()) {
        qWarning() << "Client::sendPacket: Failed! Socket not connected";
        return;
    }
    if(clientPacket.isEmpty()) {
        qWarning() << "Client::sendPacket: Invalid received data";
        return;
    }

    qint64 bytesWritten = m_serverSocket->write(clientPacket);
    if(bytesWritten == -1) {
        qWarning() << "Client::sendPacket: Failed! Write error";
        return;
    }
}

void Client::onError(QAbstractSocket::SocketError err) {
    m_reconnecting = true;
    switch(err) {
    case QAbstractSocket::HostNotFoundError:
        qWarning() << "Client::onError: Error: The host was not found.";
        return;
    case QAbstractSocket::RemoteHostClosedError:
        qWarning() << "Client::onError: Error: The remote host is closed.";
        return;
    case QAbstractSocket::ConnectionRefusedError:
        qWarning() << "Client::onError: Error: The connection was refused.";
        return;
    default:
        qWarning() << "Client::onError: Error:" << m_serverSocket->errorString();
        return;
    }
}

void Client::onDisconnected() {
    qDebug() << QString("Client::onDisconnected: Disconnected from server %1:%2")
                    .arg(m_serverAddress)
                    .arg(m_portNum);
    m_buffer.clear();

    if (m_reconnecting)
        return;

    m_reconnecting = true;

    QTimer::singleShot(5000, this, [this]() {
        m_serverSocket->connectToHost(m_serverAddress, m_portNum);
    });
}
