#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <QVariantMap>

#include "serverOpcode.h"

class Dispatcher final : public QObject {
    Q_OBJECT
private:
    explicit Dispatcher(QObject* parent = nullptr) : QObject(parent) {};
    ~Dispatcher() = default;

    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;

public:
    static Dispatcher& instance() {
        static Dispatcher instance;
        return instance;
    }

    void dispatch(ServerOpcode command, const QVariantMap& data);

signals:
    void loginReceived(QVariantMap data);
    void registryReceived(QVariantMap data);
    void updateReceived(QVariantMap data);
    void getReceived(QVariantMap data);
};

#endif // DISPATCHER_H
