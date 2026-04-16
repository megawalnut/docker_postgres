#include "server.h"

Server::Server(std::shared_ptr<DBHelper> db,
               const uint32_t portNum,
               QObject* parent)
    :
    m_db(db),
    m_port(portNum),
    QTcpServer(parent)
{
    if(!listen(QHostAddress::Any, m_port)) {
        qWarning() << "Server::Server Error";
        qWarning() << QString("Server::Unable to start the server: %1")
                        .arg(errorString());
        close();
        return;
    }

    qDebug() << QString("Server::Server started on port %1")
                        .arg(m_port);

    connect(this, &QTcpServer::newConnection,
            this, &Server::slotNewConnection);
}

void Server::slotNewConnection() {
    while(hasPendingConnections()) {
        //creating thread for the client
        QThread* thread = new QThread(nullptr);

        QTcpSocket* socket = nextPendingConnection();
        socket->setParent(nullptr);


        qintptr socketDescriptor = socket->socketDescriptor();
        socket->moveToThread(thread);
        connect(thread, &QThread::finished, socket, &QObject::deleteLater);

        ClientConnection* newClient = new ClientConnection(socketDescriptor);
        newClient->setParent(nullptr);
        newClient->moveToThread(thread);

        connect(thread, &QThread::started,
                newClient, &ClientConnection::init);

        //deleteLater
        connect(thread, &QThread::finished,
                newClient, &QObject::deleteLater);
        connect(thread, &QThread::finished,
                thread, &QObject::deleteLater);
        connect(newClient, &ClientConnection::disconnected,
                this, [this, newClient, thread](){
            thread->quit();
            m_clients.removeOne(newClient);
            qWarning() << "Server::The client is disconnected";
        }, Qt::QueuedConnection);

        //add new client in the list
        m_clients.append(newClient);
        thread->start();
    }
}

Server::~Server() {
    if(isListening()) {
        qWarning() << "Server::Stopping listening on port";
        close();
    }
}
