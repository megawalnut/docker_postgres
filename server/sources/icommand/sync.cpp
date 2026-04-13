#include "sync.h"
#include "utils.h"
#include "dispatcher.h"

// ── Ключи пакета от клиента (совпадают с ServerQueryKeys) ──────────────────
static const QString K_TABLE   = "tableName";
static const QString K_CHANGES = "changes";
static const QString K_OP      = "operation";

// ── Ключи структур PacketStructure ────────────────────────────────────────
static const QString K_COLUMNS = "columns";
static const QString K_VALUES  = "values";
static const QString K_ROWS    = "rows";
static const QString K_ROWID   = "rowId";

// ── PacketStructure::Structures (должны совпадать с клиентом) ─────────────
enum class Op : int {
    Unknown    = 0,
    Insert     = 1,
    Update     = 2,
    Remove     = 3,
    Clear      = 4,
    BulkInsert = 5
};

// ── Вспомогательные функции ────────────────────────────────────────────────

static bool applyInsert(const std::unique_ptr<DBHelper>& db, const QVariantMap& m)
{
    QString         tableName = m.value(K_TABLE).toString();
    QStringList     columns   = m.value(K_COLUMNS).toStringList();
    QVariantList    values    = m.value(K_VALUES).toList();

    if (tableName.isEmpty() || columns.isEmpty() || values.isEmpty()) {
        qWarning() << "Sync::Insert — invalid data";
        return false;
    }
    if (columns.size() != values.size()) {
        qWarning() << "Sync::Insert — columns/values size mismatch";
        return false;
    }

    // Экранируем строки одинарными кавычками
    QStringList escapedValues;
    for (const auto& v : values) {
        if (v.isNull())
            escapedValues << "NULL";
        else
            escapedValues << QString("'%1'").arg(v.toString().replace("'", "''"));
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                      .arg(tableName)
                      .arg(columns.join(", "))
                      .arg(escapedValues.join(", "));

    auto [ok, _] = db->send(sql);
    if (!ok) qWarning() << "Sync::Insert failed:" << sql;
    return ok;
}

static bool applyUpdate(const std::unique_ptr<DBHelper>& db, const QVariantMap& m)
{
    QString      tableName = m.value(K_TABLE).toString();
    int          rowId     = m.value(K_ROWID).toInt();
    QStringList  columns   = m.value(K_COLUMNS).toStringList();
    QVariantList values    = m.value(K_VALUES).toList();

    if (tableName.isEmpty() || rowId <= 0 || columns.isEmpty()) {
        qWarning() << "Sync::Update — invalid data";
        return false;
    }
    if (columns.size() != values.size()) {
        qWarning() << "Sync::Update — columns/values size mismatch";
        return false;
    }

    QStringList setParts;
    for (int i = 0; i < columns.size(); ++i) {
        const auto& v = values[i];
        QString val = v.isNull()
                          ? "NULL"
                          : QString("'%1'").arg(v.toString().replace("'", "''"));
        setParts << QString("%1 = %2").arg(columns[i]).arg(val);
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE id = %3")
                      .arg(tableName)
                      .arg(setParts.join(", "))
                      .arg(rowId);

    auto [ok, _] = db->send(sql);
    if (!ok) qWarning() << "Sync::Update failed:" << sql;
    return ok;
}

static bool applyRemove(const std::unique_ptr<DBHelper>& db, const QVariantMap& m)
{
    QString tableName = m.value(K_TABLE).toString();
    int     rowId     = m.value(K_ROWID).toInt();

    if (tableName.isEmpty() || rowId <= 0) {
        qWarning() << "Sync::Remove — invalid data";
        return false;
    }

    QString sql = QString("DELETE FROM %1 WHERE id = %2").arg(tableName).arg(rowId);
    auto [ok, _] = db->send(sql);
    if (!ok) qWarning() << "Sync::Remove failed:" << sql;
    return ok;
}

static bool applyClear(const std::unique_ptr<DBHelper>& db, const QVariantMap& m)
{
    QString tableName = m.value(K_TABLE).toString();
    if (tableName.isEmpty()) {
        qWarning() << "Sync::Clear — no table name";
        return false;
    }

    QString sql = QString("DELETE FROM %1").arg(tableName);
    auto [ok, _] = db->send(sql);
    if (!ok) qWarning() << "Sync::Clear failed:" << sql;
    return ok;
}

static bool applyBulkInsert(const std::unique_ptr<DBHelper>& db, const QVariantMap& m)
{
    QString      tableName = m.value(K_TABLE).toString();
    QStringList  columns   = m.value(K_COLUMNS).toStringList();
    QVariantList rows      = m.value(K_ROWS).toList();

    if (tableName.isEmpty() || columns.isEmpty()) {
        qWarning() << "Sync::BulkInsert — invalid data";
        return false;
    }
    if (rows.isEmpty())
        return true; // ничего вставлять не нужно

    for (const auto& rowVar : rows) {
        QVariantList rowValues = rowVar.toList();
        if (rowValues.size() != columns.size()) {
            qWarning() << "Sync::BulkInsert — columns/values mismatch in row";
            return false;
        }

        QStringList escaped;
        for (const auto& v : rowValues) {
            escaped << (v.isNull()
                            ? "NULL"
                            : QString("'%1'").arg(v.toString().replace("'", "''")));
        }

        QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                          .arg(tableName)
                          .arg(columns.join(", "))
                          .arg(escaped.join(", "));

        auto [ok, _] = db->send(sql);
        if (!ok) {
            qWarning() << "Sync::BulkInsert row failed:" << sql;
            return false;
        }
    }
    return true;
}

// ── Строим ответ Sync: возвращаем клиенту применённые изменения ────────────
static QVariantMap buildSyncResponse(const QString& tableName,
                                     const QVariantList& changes)
{
    return QVariantMap {
        {"tableName", tableName},
        {"changes",   changes}
    };
}

// ── exec ───────────────────────────────────────────────────────────────────
QByteArray Sync::exec(QVariantMap&& data)
{
    QString      tableName = data.value(K_TABLE).toString();
    QVariantList changes   = data.value(K_CHANGES).toList();

    if (tableName.isEmpty()) {
        qWarning() << "Sync::exec — no tableName";
        return Utils::Packet::serialize(Dispatcher::Opcode::sync_,
                                        {{"tableName", tableName},
                                         {"changes",   QVariantList{}}});
    }

    // Оборачиваем всё в транзакцию
    m_ptrDb->send("BEGIN");

    QVariantList appliedChanges;
    bool allOk = true;

    for (const auto& changeVar : changes) {
        QVariantMap change = changeVar.toMap();
        Op op = static_cast<Op>(change.value(K_OP).toInt());

        bool ok = false;
        switch (op) {
        case Op::Insert:
            ok = applyInsert(m_ptrDb, change);
            break;
        case Op::Update:
            ok = applyUpdate(m_ptrDb, change);
            break;
        case Op::Remove:
            ok = applyRemove(m_ptrDb, change);
            break;
        case Op::Clear:
            ok = applyClear(m_ptrDb, change);
            break;
        case Op::BulkInsert:
            ok = applyBulkInsert(m_ptrDb, change);
            break;
        default:
            qWarning() << "Sync::exec — unknown op:" << static_cast<int>(op);
            ok = false;
        }

        if (!ok) {
            allOk = false;
            break;
        }
        appliedChanges.append(changeVar);
    }

    if (allOk) {
        m_ptrDb->send("COMMIT");
        qDebug() << QString("Sync::success table=%1 changes=%2")
                        .arg(tableName).arg(appliedChanges.size());
    } else {
        m_ptrDb->send("ROLLBACK");
        appliedChanges.clear();
        qWarning() << "Sync::transaction rolled back";
    }

    return Utils::Packet::serialize(Dispatcher::Opcode::sync_,
                                    buildSyncResponse(tableName, appliedChanges));
}
