#ifndef REGISTRY_H
#define REGISTRY_H

#include <QVariantMap>

#include "icommand.h"

class Registry : public ICommand {
public:
    Registry();
    ~Registry() override;

    //method for a registry
    void exec(QVariantMap&&) override;
};

#endif // REGISTRY_H

