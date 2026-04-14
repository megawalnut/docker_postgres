#ifndef DBHELPER_H
#define DBHELPER_H

#include <QtSql>

using DbResult = std::pair<bool, std::vector<QSqlRecord>>;
extern const QString PATH;

class DBHelper {
public:
    DBHelper(const QString& filePath = PATH);
    ~DBHelper();

    DbResult send(const QString& request,  const QVariantList& values);

private:
    QSqlDatabase m_db;    //SQLite
    QString m_connectionName;   //unique thread name
    QString m_path;
};
#endif // DBHELPER_H
