#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

#include "dbhelper.h"
#include "userInfo.h"

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(std::shared_ptr<DBHelper> db, QTcpSocket* const clientSocket);
    ~ClientConnection() = default;

    bool connected() { return m_clientSocket->state() == QAbstractSocket::ConnectedState; }
    std::shared_ptr<DBHelper> db() const { return m_db; }

    void setUser(const UserInfo& user);
    UserInfo getUser() const;

public slots:
    void onPacketReady(const QByteArray& serverResponce);
    void onReadyRead();
    void onDisconnected();

signals:
    void disconnected();
    void logged();

private:
    std::shared_ptr<DBHelper> m_db = nullptr;
    QTcpSocket* m_clientSocket = nullptr;
    QByteArray m_buffer;    //accumulator packets
    QString m_clientAddress;
    quint16 m_portNum;
    UserInfo m_user;    //struct for current user
};

#endif // CLIENTCONNECTION_H
