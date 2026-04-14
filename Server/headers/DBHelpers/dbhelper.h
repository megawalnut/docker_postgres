#ifndef DBHELPER_H
#define DBHELPER_H

#include <QtSql>

using DbResult = std::pair<bool, std::vector<QSqlRecord>>;
extern const uint32_t PORTDB;

class DBHelper {
public:
    DBHelper(const QString& dbUser,
             const QString& hostName,
             const QString& dbName,
             const QString& dbPassword);
    ~DBHelper();

    bool connect();
    DbResult send(const QString& request, const QVariantList& values);

private:
    QSqlDatabase m_db;    //postgres
    QString m_connectionName;   //unique thread name
};
#endif // DBHELPER_H
