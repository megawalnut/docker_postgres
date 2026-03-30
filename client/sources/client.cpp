#include "client.h"

Client::Client(const quint16 portNum, const QString& serverAddress, QWidget* parent) : QWidget(parent), m_portNum(portNum), m_serverAddress(serverAddress) {
    m_serverSocket = new QTcpSocket(this);
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
    qDebug() << QString("Client::Client connected to server on address %1 : %2")
                    .arg(m_serverAddress)
                    .arg(m_portNum);
}

void Client::onReadyRead() {
    //read from socket and save
    m_buffer.append(m_serverSocket->readAll());

    //if received a part of the package < 4b
    while(m_buffer.size() >= sizeof(uint32_t)) {
        uint32_t realPackageSize;
        memcpy(&realPackageSize, m_buffer.constData(), sizeof(uint32_t));
        realPackageSize = qFromLittleEndian(realPackageSize);

        //if received a part of the package
        if(m_buffer.size() < realPackageSize)
            break; //wait full packet

        qDebug() << QString("The package came from a %1 : %2")
                        .arg(m_serverAddress)
                        .arg(m_portNum);

        QByteArray acceptedPackage = m_buffer.left(realPackageSize);   //package data
        m_buffer.remove(0, realPackageSize);

        emit packetReady(acceptedPackage);
    }
}

void Client::sendPacket(const QByteArray& receivedPacket) {
    if(!m_serverSocket || m_serverSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Client::failed!Socket not connected";
        return;
    }
    if(receivedPacket.isEmpty()) {
        qDebug() << "Client::Invalid received data";
        return;
    }

    qint64 bytesWritten = m_serverSocket->write(receivedPacket);
    if(bytesWritten == -1) {
        qDebug() << "Client::failed!Write error";
        return;
    }

    bool flushed = m_serverSocket->flush();
    qDebug() << (flushed ? "Client::success!Writed"
                         : "Client::failed!Flush error");
}


void Client::onError(QAbstractSocket::SocketError err) {
    switch(err) {
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error: The host was not found.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "Error: The remote host is closed.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error: The connection was refused.";
        break;
    default:
        qDebug() << "Error:" << m_serverSocket->errorString();
        break;
    }
}

void Client::onDisconnected() {
    qDebug() << QString("Client::Disconnected from server %1 : %2")
                    .arg(m_serverAddress)
                    .arg(m_portNum);
}
