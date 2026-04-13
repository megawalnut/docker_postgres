#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>

namespace TableModelKeys {
    inline const QString Id = "id";
    inline const QString Column = "column";
    inline const QString Value = "value";
}

class TableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    struct ChangeStruct {
        int id = -1;
        int column = -1;
        QVariant value;

        QVariantMap toMap() {
            return {
                {TableModelKeys::Id, id},
                {TableModelKeys::Column, column},
                {TableModelKeys::Value, value}
            };
        }
    };

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
        Type type;
        QString name;
        QString brand;
        QString model;
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

    QString getTableName() const;
    void setTableName(const QString& name);

private:
    static uint32_t counter;
    QString m_tableName;
    QList<Record> m_tableData;
};

#endif // TABLEMODEL_H
