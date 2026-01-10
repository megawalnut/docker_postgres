#include "clientConnection.h"

ClientConnection::ClientConnection(QTcpSocket* socket) : m_clientSocket(socket) {
    m_pdisp = Dispatcher::instance();
}

ClientConnection::~ClientConnection() {}

void ClientConnection::onReadyRead() {

}
