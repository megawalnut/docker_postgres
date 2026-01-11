#include "server.h"

Server::Server(const uint32_t portNum, QObject* parent) : QTcpServer(parent) {
    if(!listen(QHostAddress::Any, portNum)) {
        QMessageBox::critical(0, "Server Error", QString("Unable to start the server: %1").arg(errorString()));
        close();
        return;
    }

    qDebug() << "Server started on port" << portNum;

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
            qDebug() << "the client is disconnected";
        });

        //add new client in the list
        m_clients.append(newClient);

        qDebug() << "the client is connected from"
                 << socket->peerAddress()
                 << ":"
                 << socket->peerPort();

        thread->start();
    }
}

Server::~Server() {
    if(isListening()) {
        close();
    }
}
