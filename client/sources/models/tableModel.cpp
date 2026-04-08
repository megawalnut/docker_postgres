#include "../models/tableModel.h"

uint32_t TableModel::counter = 0;

TableModel::TableModel(QObject *parent) : QAbstractTableModel{parent} {}

int TableModel::rowCount(const QModelIndex &parent) const {
    qDebug() << "TableData::rowCount";
    if (parent.isValid()) {
        qWarning() << "TableModel::rowCount:: Invalid row";
        return 0;
    }
    return m_tableData.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
    qDebug() << "TableData::columnCount";
    if (parent.isValid()) {
        qWarning() << "TableModel::columnCount:: Invalid row";
        return 0;
    }
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    qDebug() << "TableData::data";
    if (!index.isValid()) {
        qWarning() << "TableData::data: Invalid index";
        return {};
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        qWarning() << "TableData::data: Invalid role";
        return {};
    }

    if(index.row() < 0 || index.row() >= m_tableData.size()) {
        qWarning() << "TableData::data: Index out of range";
        return {};
    }

    const auto& item = m_tableData[index.row()];

    switch (static_cast<Column>(index.column())) {
    case Column::Id: return item.id;
    case Column::Type: {
        switch (item.type) {
        case Car: return "Car";
        case Boat: return "Boat";
        case Bicycle: return "Bicycle";
        default: return "Unknown";
        }
    }
    case Column::Name: return item.name;
    case Column::Brand: return item.brand;
    case Column::Model: return item.model;
    case Column::Year: return item.year;
    }

    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    qDebug() << "TableModel::headerData";
    if (role != Qt::DisplayRole) {
        qWarning() << "TableData::headerData: Invalid role";
        return {};
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Column>(section)) {
        case Column::Id:    return "ID";
        case Column::Type:  return "Type";
        case Column::Name:  return "Name";
        case Column::Brand: return "Brand";
        case Column::Model: return "Model";
        case Column::Year:  return "Year";
        }
    }

    return {};
}

int TableModel::insertRow() {
    qDebug() << "TableModel::insertRow";

    int row = m_tableData.size();

    beginInsertRows(QModelIndex{}, row, row);
    m_tableData.append(Record{});
    m_dataChanged = AppContext::SyncState::Synced;
    emit syncStateChanged(m_dataChanged);
    endInsertRows();

    return m_tableData[row].id;
}

int TableModel::deleteRow(int selectedRow) {
    qDebug() << "TableModel::deleteRow";
    if(selectedRow < 0 || selectedRow >= m_tableData.size()) {
        qWarning() << "TableModel::deleteRow:: Invalid row";
        return -1;
    }

    int id = m_tableData[selectedRow].id;

    beginRemoveRows(QModelIndex{}, selectedRow, selectedRow);
    m_tableData.removeAt(selectedRow);
    m_dataChanged = AppContext::SyncState::Synced;
    emit syncStateChanged(m_dataChanged);
    endRemoveRows();

    return id;
}

QVariantMap TableModel::changeField(int selectedRow,  int selectedCol, const QVariant& value) {
    qDebug() << "TableModel::changeField";

    QVariantMap data;
    data["id"] = -1;
    data["column"] = -1;
    data["field"] = QVariant{};

    if(selectedRow < 0 || selectedRow >= m_tableData.size()) {
        qWarning() << "TableModel::changeField:: Invalid row";
        return data;
    }

    auto& item = m_tableData[selectedRow];

    switch(static_cast<Column>(selectedCol)){
    case Column::Id:    item.id = value.toUInt(); break;
    case Column::Type:  item.type = static_cast<Type>(value.toUInt()); break;
    case Column::Name:  item.name = value.toString(); break;
    case Column::Brand: item.brand = value.toString(); break;
    case Column::Model: item.model = value.toString(); break;
    case Column::Year:  item.year = value.toUInt(); break;
    }

    emit dataChanged(index(selectedRow, selectedCol),
                     index(selectedRow, selectedCol),
                     {Qt::EditRole});

    m_dataChanged = AppContext::SyncState::Synced;
    emit syncStateChanged(m_dataChanged);

    data["id"] = item.id;
    data["column"] = selectedCol;
    data["field"] = value;

    return data;
}

AppContext::SyncState TableModel::getSyncState() const {
    qDebug() << "TableModel::getSyncState";
    return m_dataChanged;
}

void TableModel::setSyncState(AppContext::SyncState status) {
    qDebug() << "TableModel::setSyncState";
    m_dataChanged = status;
}

QString TableModel::getTableName() const {
    qDebug() << "TableModel::getTableName";
    return m_tableName;
}

void TableModel::setTableName(const QString& name) {
    qDebug() << "TableModel::setTableName";
    m_tableName = name;
}
