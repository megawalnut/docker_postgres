#include "../Models/tableModel.h"

uint32_t TableModel::counter = 0;

TableModel::TableModel(QObject *parent) : QAbstractTableModel{parent} {}

int TableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        qWarning() << "TableModel::rowCount:: Invalid row";
        return 0;
    }
    return m_tableData.size();
}
int TableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        qWarning() << "TableModel::columnCount:: Invalid row";
        return 0;
    }
    return 6;
}
QVariant TableModel::data(const QModelIndex &index, int role) const {
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
    case Column::Type: return item.type;
    case Column::Name: return item.name;
    case Column::Brand: return item.brand;
    case Column::Model: return item.model;
    case Column::Year: return item.year;
    }

    return {};
}
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable
           | Qt::ItemIsEnabled
           | Qt::ItemIsEditable;
}
bool TableModel::setData(const QModelIndex &index,
                         const QVariant &value,
                         int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    changeField(index.row(), index.column(), value);

    return true;
}
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
    m_tableData.append(Record{++counter});
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
    endRemoveRows();

    return id;
}
QVariantMap TableModel::changeField(int selectedRow,  int selectedCol, const QVariant& value) {
    qDebug() << "TableModel::changeField";

    ChangeStruct data;

    if(selectedRow < 0 || selectedRow >= m_tableData.size()) {
        qWarning() << "TableModel::changeField:: Invalid row";
        return data.toMap();
    }

    auto& item = m_tableData[selectedRow];

    switch(static_cast<Column>(selectedCol)){
    case Column::Id:    item.id = value.toUInt(); break;
    case Column::Type:  item.type = value.toString(); break;;
    case Column::Name:  item.name = value.toString(); break;
    case Column::Brand: item.brand = value.toString(); break;
    case Column::Model: item.model = value.toString(); break;
    case Column::Year:  item.year = value.toUInt(); break;
    }

    emit dataChanged(index(selectedRow, selectedCol),
                     index(selectedRow, selectedCol),
                     {Qt::EditRole});

    data.id = item.id;
    data.column = selectedCol;
    data.value = value;

    return data.toMap();
}
void TableModel::loadFromBulkInsert(const PacketStructure::BulkInsert& data) {
    beginResetModel();
    m_tableData.clear();

    for (const auto& row : data.rows) {
        if (row.size() < 6) {
            continue;
        }Record rec;
        rec.id    = row[0].toUInt();
        rec.type  = row[1].toString();
        rec.name  = row[2].toString();
        rec.brand = row[3].toString();
        rec.model = row[4].toString();
        rec.year  = row[5].toUInt();
        m_tableData.append(rec);
    }
    endResetModel();
}

void TableModel::clear() {
    m_tableData.clear();
}
QString TableModel::getTableName() const {
    return m_tableName;
}
void TableModel::setTableName(const QString& name) {
    m_tableName = name;
}
