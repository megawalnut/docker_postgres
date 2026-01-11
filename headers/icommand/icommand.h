#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QVariantMap>

class ICommand {
public:
    virtual ~ICommand() = default;

    //method for a specific command
    virtual void exec(QVariantMap&&) = 0;
};

#endif // ICOMMAND_H
