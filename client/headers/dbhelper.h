#ifndef DBHELPER_H
#define DBHELPER_H

#include <QtSql>

using DbResult = std::pair<bool, std::vector<QSqlRecord>>;

class DBHelper final {
public:
    DBHelper(const QString& filePath);
    ~DBHelper();

    DbResult send(const QString& request,  const QVariantList& values);

private:
    QSqlDatabase m_db;    //SQLite
    QString m_path;
};
#endif // DBHELPER_H
