#ifndef STASHITEMSTABLE_H
#define STASHITEMSTABLE_H

#include <QObject>
#include <sqlitetable.h>

struct StashItem
{
    qint64 itemId;
    qint64 groupId;
    QByteArray item;

    StashItem()
    {
        itemId = 0;
        groupId = 0;
    }
};

class StashItemsTable : public SqliteTable
{
public:
    explicit StashItemsTable(QString databaseName);

    void DatabasePath(QString filePath);

    qint64 AddStashItem(StashItem item);
    StashItem GetStashItem(qint64 itemId);
    QList<StashItem> GetItemsInGroup(qint64 groupId);
    bool UpdateGroup(StashItem item);
    bool DeleteStashItem(qint64 itemId);
    
signals:
    
public slots:


private:
    static const QString StashItemId;
    static const QString GroupId;
    static const QString ItemName;
    static const QString DamageAttributes;
    static const QString ArmorAttributes;
    static const QString MagicAttributes;
    static const QString StashItemData;

    StashItem ParseRecord(QSqlRecord inRecord);
    
};

#endif // STASHITEMSTABLE_H
