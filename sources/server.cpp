#include "server.h"

Server::Server(const uint32_t portNum, QObject* parent) : QTcpServer(parent) {
    if(!listen(QHostAddress::Any, portNum)) {
        qDebug() << "Server::Server Error";
        qDebug() << QString("Server::Unable to start the server: %1")
                            .arg(errorString());
        close();
        return;
    }

    qDebug() << QString("Server::Server started on port %1")
                        .arg(portNum);

    connect(this, &QTcpServer::newConnection,
            this, &Server::slotNewConnection);
}

void Server::slotNewConnection() {
    while(hasPendingConnections()) {
        //creating thread for the client
        QThread* thread = new QThread(nullptr);
        QTcpSocket* socket = nextPendingConnection();
        ClientConnection* newClient = new ClientConnection(socket);

        //move in thread
        socket->moveToThread(thread);
        newClient->moveToThread(thread);

        connect(socket, &QTcpSocket::readyRead,
                newClient, &ClientConnection::onReadyRead);

        connect(socket, &QTcpSocket::disconnected,
                newClient, &ClientConnection::onDisconnected);

        connect(newClient, &ClientConnection::disconnected, [this, newClient, &thread](){
            m_clients.removeOne(newClient);
            newClient->deleteLater();
            thread->quit();
            thread->deleteLater();
            qDebug() << "Server::The client is disconnected";
        });

        //add new client in the list
        m_clients.append(newClient);

        qDebug() << QString("Server::The client is connected from %1 : %2")
                            .arg(socket->peerAddress().toString())
                            .arg(socket->peerPort());

        thread->start();
    }
}

Server::~Server() {
    if(isListening()) {
        qDebug() << "Server::Stopping listening on port";
        close();
    }
}
