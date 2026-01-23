#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(QTcpSocket* const clientSocket);
    ~ClientConnection();

public slots:
    void onDisconnected();
    void onReadyRead();
    void onPacketReady(const QByteArray& receivedPacket);

signals:
    void disconnected();
    void packetReady(const QByteArray& receivedPacket);

private:
    QTcpSocket* m_clientSocket;
    QByteArray m_buffer;   //accumulator
};

#endif // CLIENTCONNECTION_H
