#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>

class Client final : public QObject {
    Q_OBJECT
public:
    explicit Client(const quint16 portNum, const QString& serverAddress, QObject* parent = nullptr);
    ~Client() = default;

    void sendPacket(const QByteArray& clientPacket);

    bool connected() {
        return m_serverSocket->state() == QAbstractSocket::ConnectedState;
    }

private slots:
    void onReadyRead();
    void onConnected();
    void onError(QAbstractSocket::SocketError);
    void onDisconnected();

signals:
    void packetReady(QByteArray rawData);

private:
    QTcpSocket* m_serverSocket = nullptr;
    bool m_reconnecting;        //if we watn to reconnecting
    QByteArray m_buffer;        //accumulator packets
    QString m_serverAddress;    //"127.0.0.1"
    quint16 m_portNum;          //5555
};

#endif // CLIENT_H
