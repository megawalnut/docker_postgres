#include "../models/tableModel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel{parent} {
    qDebug() << "TableData::TableModel";

}

int TableModel::rowCount(const QModelIndex &parent) const {
    qDebug() << "TableData::rowCount";
    if (parent.isValid())
        return 0;
    return m_tableData.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
    qDebug() << "TableData::columnCount";
    if (parent.isValid())
        return 0;
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    qDebug() << "TableData::data";
    if (!index.isValid())
        return {};

    if(index.row() < 0 || index.row() >= m_tableData.size()) {
        qDebug() << "TableData::data: Index out of range";
        return {};
    }

    const auto& item = m_tableData[index.row()];
    switch (static_cast<Column>(index.column())) {
    case Column::Id: return item.id;
    case Column::Type:
        switch (item.type) {
        case Car: return "Car";
        case Boat: return "Boat";
        case Bicycle: return "Bicycle";
        default: return "Unknown";
        }
    case Column::Name: return item.name;
    case Column::Brand: return item.brand;
    case Column::Model: return item.model;
    case Column::Year: return item.year;
    }

    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Column>(section)) {
        case Column::Id: return "ID";
        case Column::Type: return "Type";
        case Column::Name: return "Name";
        case Column::Brand: return "Brand";
        case Column::Model: return "Model";
        case Column::Year: return "Year";
        }
    }

    return {};
}
