#ifndef DBHELPER_H
#define DBHELPER_H

#include <QtSql>

using DbResult = std::pair<bool, std::vector<QSqlRecord>>;

class DBHelper {
public:
    DBHelper(const QString& userName,
             const QString& hostName,
             const QString& dbName,
             const QString& password);
    ~DBHelper();

    void connect();
    DbResult send(const QString& request) const;

private:
    QSqlDatabase m_db;    //postgres
};
#endif // DBHELPER_H
