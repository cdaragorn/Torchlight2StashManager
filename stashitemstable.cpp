#include "stashitemstable.h"

#include <torchlight2item.h>

const QString StashItemsTable::StashItemId = "stashItemsId";
const QString StashItemsTable::GroupId = "groupId";
const QString StashItemsTable::ItemName = "itemName";
const QString StashItemsTable::DamageAttributes = "damageAttributes";
const QString StashItemsTable::ArmorAttributes = "armorAttributes";
const QString StashItemsTable::MagicAttributes = "magicAttributes";
const QString StashItemsTable::StashItemData = "stashItemData";

StashItemsTable::StashItemsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void StashItemsTable::DatabasePath(QString filePath)
{
    DatabaseName(filePath);

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (!query.exec("CREATE TABLE IF NOT EXISTS stashItems (" +
                        StashItemId + " INTEGER PRIMARY KEY, " +
                        GroupId + " INTEGER NOT NULL, " +
                        ItemName + " TEXT, " +
                        DamageAttributes + " TEXT, " +
                        ArmorAttributes + " TEXT, " +
                        MagicAttributes + " TEXT, " +
                        StashItemData + " BLOB" +
                        ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS " + GroupId + "Index ON stashItems (" + GroupId + ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS " + ItemName + "Index ON stashItems (" + ItemName + ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS " + DamageAttributes + "Index ON stashItems (" + DamageAttributes + ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS " + ArmorAttributes + "Index ON stashItems (" + ArmorAttributes + ")"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS " + MagicAttributes + "Index ON stashItems (" + MagicAttributes + ")"))
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }
}

StashItem StashItemsTable::ParseRecord(QSqlRecord inRecord)
{
    StashItem result;

    if (!inRecord.isEmpty())
    {
        QSqlField itemId = inRecord.field(StashItemId);
        QSqlField groupId = inRecord.field(GroupId);
        QSqlField itemData = inRecord.field(StashItemData);

        result.itemId = itemId.value().toLongLong();
        result.groupId = groupId.value().toLongLong();
        result.item = itemData.value().toByteArray();

    }

    return result;
}

qint64 StashItemsTable::AddStashItem(StashItem item)
{
    qint64 result = 0;

    if (item.groupId > 0)
    {
        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("INSERT INTO stashItems (" + GroupId + ", " + ItemName + ", " + DamageAttributes + ", " +
                              ArmorAttributes + ", " + MagicAttributes + ", " + StashItemData +
                              ") VALUES (:groupId, :itemName, :damageAttributes, :armorAttributes, " +
                              ":magicAttributes, :stashItemData)"))
            {
                Torchlight2Item itemDetails(item.item);

                query.bindValue(":groupId", item.groupId);
                query.bindValue(":itemName", itemDetails.Name());
                query.bindValue(":damageAttributes", "");
                query.bindValue(":armorAttributes", "");
                query.bindValue(":magicAttributes", "");
                query.bindValue(":stashItemData", item.item);

                if (query.exec())
                {
                    if (!query.lastInsertId().isNull())
                    {
                        result = query.lastInsertId().toLongLong();
                    }
                }
                else
                {
                    PrintSqlError(query.lastError());
                }


            }
            else
            {
                PrintSqlError(query.lastError());
            }

            db.close();
        }
    }

    return result;
}


bool StashItemsTable::DeleteStashItem(qint64 itemId)
{
    bool result = false;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("DELETE FROM stashItems WHERE " + StashItemId + " = :itemId"))
        {
            query.bindValue(":itemId", itemId);

            result = query.exec();
        }

        if (!result)
            PrintSqlError(query.lastError());

        db.close();
    }

    return result;
}


StashItem StashItemsTable::GetStashItem(qint64 itemId)
{
    StashItem result;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("SELECT * FROM stashItems WHERE " + StashItemId + " = :stashItemId"))
        {
            query.bindValue(":stashItemId", itemId);

            if (query.exec())
            {
                if (query.next())
                {
                    result = ParseRecord(query.record());
                }
            }
            else
            {
                PrintSqlError(query.lastError());
            }
        }
        else
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }

    return result;
}


QList<StashItem> StashItemsTable::GetItemsInGroup(qint64 groupId)
{
    QList<StashItem> result;

    if (groupId > 0)
    {
        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("SELECT * FROM stashItems WHERE " + GroupId + " = :groupId"))
            {
                query.bindValue(":groupId", groupId);

                if (query.exec())
                {
                    while (query.next())
                    {
                        StashItem nextItem = ParseRecord(query.record());
                        result.append(nextItem);
                    }
                }
                else
                {
                    PrintSqlError(query.lastError());
                }
            }
            else
            {
                PrintSqlError(query.lastError());
            }

            db.close();
        }
    }

    return result;
}

bool StashItemsTable::UpdateGroup(StashItem item)
{
    bool result = false;

    if (item.groupId > 0)
    {
        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("UPDATE stashItems SET " + GroupId + " = :groupId WHERE " + StashItemId + " = :itemId"))
            {
                query.bindValue(":groupId", item.groupId);
                query.bindValue(":itemId", item.itemId);

                result = query.exec();
            }

            if (!result)
                PrintSqlError(query.lastError());

            db.close();
        }

    }

    return result;
}
