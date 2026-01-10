#include "server.h"

Server::Server(uint32_t portNum, QObject* parent) : QTcpServer(parent) {
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
        QTcpSocket* socket = nextPendingConnection();
        ClientConnection* newClient = new ClientConnection(socket, this);
        m_clients.append(newClient);
        qDebug() << "The client is connected from" << socket->peerAddress() << ":" << socket->peerPort();

        connect(newClient, &ClientConnection::disconnected,[this, newClient](){
            m_clients.removeOne(newClient);
            newClient->deleteLater();
            qDebug() << "The client is disconnected";
        });
    }
}

Server::~Server() {
    if(isListening()) {
        close();
    }
}
