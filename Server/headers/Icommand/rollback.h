#ifndef ROLLBACK_H
#define ROLLBACK_H

#include <QVariantMap>

#include "icommand.h"

class Rollback : public ICommand {
public:
    Rollback() = default;
    ~Rollback() = default;

    //method for a rollback
    QVariantMap exec(const QVariantMap& data,
                     ClientConnection* m_client) override;
};

#endif // ROLLBACK_H


