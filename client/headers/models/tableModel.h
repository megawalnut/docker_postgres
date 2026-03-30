#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

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
        uint32_t id;
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

private:
    QList<Record> m_tableData;
};

#endif // TABLEMODEL_H
