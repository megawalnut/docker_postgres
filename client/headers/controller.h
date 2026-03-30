#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QVariantMap>

#include "serverOpcode.h"
#include "localOpcode.h"
#include "client.h"

class Controller : public QObject {
    Q_OBJECT
    using localOpcode = LocalOpcode;
    using serverOpcode = ServerOpcode;
public:
    explicit Controller(Client* client, QObject *parent = nullptr);

private slots:
    void onPacketReady(QByteArray data);

signals:
    void dispatch(LocalOpcode opcode, QVariantMap data);

private:
    Client* m_client = nullptr;
};

#endif // CONTROLLER_H
