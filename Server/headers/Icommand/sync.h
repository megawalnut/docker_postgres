#ifndef SYNC_H
#define SYNC_H

#include <QVariantMap>

#include "icommand.h"

class Sync : public ICommand {
public:
    Sync() = default;
    ~Sync() = default;

    //method for a sync
    QVariantMap exec(const QVariantMap& data,
                     ClientConnection* m_client) override;
};

#endif // SYNC_H

