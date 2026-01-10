#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

#include "dispatcher.h"

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(QTcpSocket* clientSocket, QObject* parent = nullptr);
    ~ClientConnection();

    enum opcode {
        None = 0,
        Autorisation,
        Registry,
        Send,
        Receive
    };

private slots:
    void onDisconnected() {
        emit disconnected();
    }

signals:
    void disconnected();

private:
    QTcpSocket* m_clientSocket;
    Dispatcher* m_pdisp;
};

#endif // CLIENTCONNECTION_H
