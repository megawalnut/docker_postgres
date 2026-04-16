#include "clientConnection.h"
#include "utils.h"
#include "dispatcher.h"

ClientConnection::ClientConnection(quintptr socketDescriptor,
                                   QObject* parent)
    :
    QObject(parent),
    m_socketDescriptor(socketDescriptor)
{}

void ClientConnection::init() {
    m_clientSocket = new QTcpSocket(this);

    if (!m_clientSocket->setSocketDescriptor(m_socketDescriptor)) {
        qWarning() << "ClientConnection::init: Failed:"
                   << m_clientSocket->errorString();
        emit disconnected();
        return;
    }

    m_db = std::make_unique<DBHelper>(
        "admin",
        "127.0.0.1",
        "test_database",
        "root"
        );

    if (!m_db->connect()) {
        qWarning() << "DB connect failed in thread";
        emit disconnected();
        return;
    }

    m_clientAddress = m_clientSocket->peerAddress().toString();
    m_portNum = m_clientSocket->peerPort();

    connect(m_clientSocket, &QTcpSocket::readyRead,
            this, &ClientConnection::onReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected,
            this, &ClientConnection::onDisconnected);

    qDebug() << "ClientConnection::init: Success on "
             << m_clientAddress << ":" << m_portNum;
}

void ClientConnection::setUser(const UserInfo& user) {
    m_user = user;
}
UserInfo ClientConnection::getUser() const {
    return m_user;
}

void ClientConnection::onDisconnected() {
    m_clientSocket->close();
    emit disconnected();
}

void ClientConnection::onReadyRead() {
    qDebug() << "ClientConnection::onReadyRead()";
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
            qWarning() << "ClientConnection::onReadyRead: Too big packet";
            m_buffer.clear();
            return;
        }

        //if received a part of the package
        if(m_buffer.size() < realPackageSize) {
            break; //wait full packet
        }

        if (realPackageSize < MIN_PACKET_SIZE) {
            qWarning() << "ClientConnection::onReadyRead: Invalid packet";
            m_buffer.clear();
            return;
        }

        qDebug() << QString("ClientConnection::onReadyRead: The package came from a %1:%2")
                        .arg(m_clientAddress)
                        .arg(m_portNum);


        QByteArray acceptedPackage = m_buffer.left(realPackageSize);   //package data
        m_buffer.remove(0, realPackageSize);

        const auto& [op, data] = Utils::Packet::deserialize(acceptedPackage);

        const QVariantMap& serverResponce = Dispatcher::instance().dispatch(op, data, this);

        onPacketReady(Utils::Packet::serialize(op, serverResponce));
    }
}

void ClientConnection::onPacketReady(const QByteArray& serverResponce) {
    if(!connected()) {
        qWarning() << "ClientConnection::onPacketReady: Failed!Socket not connected";
        return;
    }
    if(serverResponce.isEmpty()) {
        qWarning() << "ClientConnection:::onPacketReady: Invalid received data";
        return;
    }

    qint64 bytesWritten = m_clientSocket->write(serverResponce);
    if(bytesWritten == -1) {
        qWarning() << "ClientConnection:::onPacketReady: Failed!Write error";
        return;
    }
}
