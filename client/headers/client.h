#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QtEndian>

class Client: public QWidget {
    Q_OBJECT
public:
    explicit Client(const quint16 portNum, const QString& serveraddress, QWidget* parent = nullptr);
    ~Client() = default;

    void sendPacket(const QByteArray& packet);

private slots:
    void onReadyRead();
    void onConnected();
    void onError(QAbstractSocket::SocketError);
    void onDisconnected();

signals:
    void packetReady(QByteArray rawData);

private:
    QTcpSocket* m_serverSocket;
    QByteArray  m_buffer;           //accumulator
    QString     m_serverAddress;    //"127.0.0.1"
    quint16     m_portNum;          //5555
};

#endif // CLIENT_H
