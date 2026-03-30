#ifndef UPDATE_H
#define UPDATE_H

#include <QVariantMap>

#include "icommand.h"
#include "dbhelper.h"

class Update : public ICommand {
public:
    explicit Update(DBHelper* db);

    //method for a Update
    QByteArray exec(const QVariantMap& data) override;

private:
    DBHelper* m_ptrDb;
};

#endif // UPDATE_H
