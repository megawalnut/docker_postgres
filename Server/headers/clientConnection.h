#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>

#include "dbhelper.h"
#include "userInfo.h"

class ClientConnection : public QObject {
    Q_OBJECT
public:
    explicit ClientConnection(quintptr socketDescriptor,
                              QObject* parent = nullptr);
    ~ClientConnection() = default;

    bool connected() { return m_clientSocket->state() == QAbstractSocket::ConnectedState; }
    DBHelper* const db() const { return m_db.get(); }

    void setUser(const UserInfo& user);
    UserInfo getUser() const;

public slots:
    void onPacketReady(const QByteArray& serverResponce);
    void onReadyRead();
    void onDisconnected();
    void init();

signals:
    void disconnected();
    void logged();

private:
    std::unique_ptr<DBHelper> m_db = nullptr;
    QTcpSocket* m_clientSocket = nullptr;
    quintptr m_socketDescriptor;
    QByteArray m_buffer;    //accumulator packets
    QString m_clientAddress;
    quint16 m_portNum;
    UserInfo m_user;    //struct for current user
};

#endif // CLIENTCONNECTION_H
