#include "controller.h"
#include "dispatcher.h"
#include "utils.h"

Controller::Controller(Client* client, QObject *parent) : QObject{parent}, m_client(client) {
    connect(m_client, &Client::packetReady,
            this, &Controller::onPacketReady);



    // connect(this, &Controller::dispatch,
    //         &Dispatcher::instance(), &Dispatcher::dispatch);
}

void Controller::onPacketReady(QByteArray rawData) {
    std::pair<const uint32_t, QVariantMap> data = Utils::Packet::deserialize(rawData);
    //emit dispatch(static_cast<ServerOpcode>(data.first), data.second);
}
