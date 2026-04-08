#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

#include "../appContext.h"

class TableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Type {
        Unknown = 0,
        Car,
        Boat,
        Bicycle
    };

    enum class Column {
        Id = 0,
        Type,
        Name,
        Brand,
        Model,
        Year
    };

    struct Record {
        uint32_t id = counter++;
        Type     type;
        QString  name;
        QString  brand;
        QString  model;
        uint32_t year;
    };

    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public:
    int insertRow();
    int deleteRow(int selectedRow);
    QVariantMap changeField(int selectedRow,  int selectedCol, const QVariant& value);

    AppContext::SyncState getSyncState() const;
    void setSyncState(AppContext::SyncState status);

    QString getTableName() const;
    void setTableName(const QString& name);

signals:
    void syncStateChanged(AppContext::SyncState);

private:
    static uint32_t counter;

    QString m_tableName;
    AppContext::SyncState m_dataChanged = AppContext::SyncState::Unsynced;
    QList<Record> m_tableData;
};

#endif // TABLEMODEL_H
