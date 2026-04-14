#ifndef FULLDUMP_H
#define FULLDUMP_H

#include <QVariantMap>

#include "icommand.h"

class FullDump : public ICommand {
public:
    FullDump() = default;
    ~FullDump() = default;

    //method for a fulldump
    QVariantMap exec(const QVariantMap& data,
                     ClientConnection* m_client) override;
};

#endif // FULLDUMP_H

