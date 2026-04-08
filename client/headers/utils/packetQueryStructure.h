#ifndef QUERYSTRUCTURE_H
#define QUERYSTRUCTURE_H

#include <QString>
#include <QVariantList>

namespace PacketQueryKeys {
inline const QString TableName = "tableName";
inline const QString Columns = "columns";
inline const QString Values = "values";
inline const QString RowId = "rowId";
inline const QString Rows = "rows";
}

namespace PacketStructure {
    struct Insert {
        QString tableName;
        QStringList columns;
        QVariantList values;

        QVariantMap toMap() const {
            return QVariantMap {
                {PacketQueryKeys::TableName, tableName},
                {PacketQueryKeys::Columns, columns},
                {PacketQueryKeys::Values, values}
            };
        };

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketQueryKeys::TableName).toString();
            columns = map.value(PacketQueryKeys::Columns).toStringList();
            values = map.value(PacketQueryKeys::Values).toList();
        };
    };
    struct Update {
        int rowId = -1;
        QString tableName;
        QStringList columns;
        QVariantList values;

        QVariantMap toMap() const {
            return QVariantMap {
                {PacketQueryKeys::TableName, tableName},
                {PacketQueryKeys::Columns, columns},
                {PacketQueryKeys::Values, values},
                {PacketQueryKeys::RowId, rowId},
            };
        };

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketQueryKeys::TableName).toString();
            columns = map.value(PacketQueryKeys::Columns).toStringList();
            values = map.value(PacketQueryKeys::Values).toList();
            rowId = map.value(PacketQueryKeys::RowId).toInt();
        };
    };
    struct Remove {
        int rowId = -1;
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {PacketQueryKeys::TableName, tableName},
                {PacketQueryKeys::RowId, rowId}
            };
        };

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketQueryKeys::TableName).toString();
            rowId = map.value(PacketQueryKeys::RowId).toInt();
        };
    };
    struct Clear {
        QString tableName;

        QVariantMap toMap() const {
            return QVariantMap {
                {PacketQueryKeys::TableName, tableName}
            };
        };

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketQueryKeys::TableName).toString();
        };
    };
    struct BulkInsert {
        QString tableName;
        QStringList columns;
        QVector<QVariantList> rows;

        QVariantMap toMap() const {
            QVariantList list = QVariantList(rows.cbegin(), rows.cend());
            return QVariantMap {
                {PacketQueryKeys::TableName, tableName},
                {PacketQueryKeys::Columns, columns},
                {PacketQueryKeys::Rows, list}
            };
        };

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketQueryKeys::TableName).toString();
            columns = map.value(PacketQueryKeys::Columns).toStringList();

            rows.clear();
            for(const auto& row : map.value(PacketQueryKeys::Rows).toList()) {
                rows.push_back(row.toList());
            };
        };
    };
}

#endif  // QUERYSTRUCTURE_H
