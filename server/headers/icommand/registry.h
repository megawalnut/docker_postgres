#ifndef REGISTRY_H
#define REGISTRY_H

#include <QVariantMap>

#include "icommand.h"

class Registry : public ICommand {
public:
    Registry() = default;
    ~Registry() = default;

    //method for a registry
    QByteArray exec(QVariantMap&&) override;
};

#endif // REGISTRY_H

