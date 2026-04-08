#include "dispatcher.h"

void Dispatcher::dispatch(ServerOpcode command, const QVariantMap& data) {
    emit serverCommand(command, data);
}

void Dispatcher::dispatch(LocalOpcode command, const QVariantMap& data) {
    emit localCommand(command, data);
}
