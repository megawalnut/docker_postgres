#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QList>
#include "clientConnection.h"

class Server: public QTcpServer {
    Q_OBJECT
public:
    explicit Server(uint32_t portNum, QObject* parent = nullptr);
    ~Server();

public slots:
    void slotNewConnection();

private:
    QList<ClientConnection*> m_clients;
};

#endif // SERVER_H
