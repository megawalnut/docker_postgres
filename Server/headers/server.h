#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QThread>

#include "clientConnection.h"

extern const uint32_t PORTSERVER;

class Server: public QTcpServer {
    Q_OBJECT
public:
    explicit Server(std::shared_ptr<DBHelper> db, const uint32_t portNum = PORTSERVER, QObject* parent = nullptr);
    ~Server();

public slots:
    void slotNewConnection();

private:
    std::shared_ptr<DBHelper> m_db;
    QList<ClientConnection*> m_clients;
    const uint32_t m_port;
};

#endif // SERVER_H
