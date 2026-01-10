#include "clientConnection.h"

ClientConnection::ClientConnection(QTcpSocket* socket, QObject* parent) : QObject(parent), m_clientSocket(socket) {
    m_pdisp = Dispatcher::instance();
}

ClientConnection::~ClientConnection() {}
