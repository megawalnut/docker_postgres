#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QVariantMap>

#include "clientConnection.h"

class ICommand {
public:
    virtual ~ICommand() = default;

    //method for a specific command
    virtual QVariantMap exec(const QVariantMap& data,
                             ClientConnection* m_client) = 0;
};

#endif // ICOMMAND_H
