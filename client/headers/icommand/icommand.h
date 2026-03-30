#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QVariantMap>

class ICommand {
public:
    //method for a specific command
    virtual QByteArray exec(const QVariantMap& data) = 0;
    virtual ~ICommand() = default;
};

#endif // ICOMMAND_H
