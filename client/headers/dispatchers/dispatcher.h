#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <QVariantMap>

#include "serverOpcode.h"
#include "localOpcode.h"

class Dispatcher final : public QObject {
    Q_OBJECT
private:
    explicit Dispatcher(QObject* parent = nullptr) : QObject(parent) {};
    ~Dispatcher() = default;

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

public:
    void dispatch(ServerOpcode command, const QVariantMap& data);   //server commands
    void dispatch(LocalOpcode command, const QVariantMap& data);    //local commands

    static Dispatcher& instance() {
        static Dispatcher instance;
        return instance;
    }

signals:
    void serverCommand(ServerOpcode command, QVariantMap data);    //server commands
    void localCommand(LocalOpcode command, QVariantMap data);    //local commands
};

#endif // DISPATCHER_H
