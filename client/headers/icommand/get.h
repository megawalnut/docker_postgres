#ifndef GET_H
#define GET_H

#include <QVariantMap>

#include "icommand.h"
#include "dbhelper.h"

class Get : public ICommand {
public:
    explicit Get(DBHelper* db);

    //method for a get
    QByteArray exec(const QVariantMap& data) override;
private:
    DBHelper* m_ptrDb;
};

#endif // GET_H


