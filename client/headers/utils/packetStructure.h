#ifndef PACKETSSTRUCTURE_H
#define PACKETSSTRUCTURE_H

#include <QString>
#include <QVariantList>

namespace PacketStructureKeys {
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

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketStructureKeys::TableName).toString();
            columns = map.value(PacketStructureKeys::Columns).toStringList();
            values = map.value(PacketStructureKeys::Values).toList();
        }
    };
    struct Change {
        int rowId = -1;
        QString tableName;
        QStringList columns;
        QVariantList values;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketStructureKeys::TableName).toString();
            columns = map.value(PacketStructureKeys::Columns).toStringList();
            values = map.value(PacketStructureKeys::Values).toList();
            rowId = map.value(PacketStructureKeys::RowId).toInt();
        }
    };
    struct Remove {
        int rowId = -1;
        QString tableName;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketStructureKeys::TableName).toString();
            rowId = map.value(PacketStructureKeys::RowId).toInt();
        }
    };
    struct Clear {
        QString tableName;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketStructureKeys::TableName).toString();
        }
    };
    struct BulkInsert { //entire table
        QString tableName;
        QStringList columns;
        QVector<QVariantList> rows;

        void fromMap(const QVariantMap& map) {
            tableName = map.value(PacketStructureKeys::TableName).toString();
            columns = map.value(PacketStructureKeys::Columns).toStringList();

            rows.clear();
            for(const auto& row : map.value(PacketStructureKeys::Rows).toList()) {
                rows.push_back(row.toList());
            };
        }
    };
}

#endif  // PACKETSSTRUCTURE_H
