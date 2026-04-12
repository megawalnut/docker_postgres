#ifndef DBCRUD_H
#define DBCRUD_H

#include <QString>

#include "dbhelper.h"
#include "../headers/utils/serverQueryStructure.h"

using namespace PacketStructure;

class DBCRUD {
public:
    explicit DBCRUD();

    bool insert(const Insert& data);
    bool update(const Update& data);
    bool remove(const Remove& data);
    bool clear(const Clear& data);

    bool bulkInsert(const BulkInsert& data);

private:
    DBHelper& m_db;
};

#endif // DBCRUD_H
