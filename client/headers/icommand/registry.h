#ifndef REGISTRY_H
#define REGISTRY_H

#include <QVariantMap>

#include "icommand.h"
#include "dbhelper.h"

class Registry : public ICommand {
public:
    explicit Registry(DBHelper* db);

    //method for a Registry
    QByteArray exec(const QVariantMap& data) override;

private:
    DBHelper* m_ptrDb;
};

#endif // REGISTRY_H
