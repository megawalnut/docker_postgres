#include "dispatcher.h"

void Dispatcher::dispatch(ServerOpcode command, const QVariantMap& data) {
    switch(command){
    case ServerOpcode::login: {
        emit loginReceived(data);
        qDebug() << "Dispatcher:loginReceived";
        break;
    }
    case ServerOpcode::registry: {
        emit registryReceived(data);
        qDebug() << "Dispatcher:registryReceived";
        break;
    }
    case ServerOpcode::update: {
        emit updateReceived(data);
        qDebug() << "Dispatcher:updateReceived";
        break;
    }
    case ServerOpcode::get: {
        emit getReceived(data);
        qDebug() << "Dispatcher:getReceived";
        break;
    }
    case ServerOpcode::unknown:
    default: {
        qWarning() << "Unknown opcode:" << static_cast<int>(command);
        break;
    }
    }
}
