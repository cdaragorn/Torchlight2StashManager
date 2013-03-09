#include "groupstable.h"

GroupsTable::GroupsTable(QString databaseName, QObject *parent) :
    SqliteTable(databaseName, parent)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        bool result = query.exec("CREATE TABLE IF NOT EXISTS groups (id INTEGER PRIMARY KEY, groupName TEXT)");
        if (!result)
        {
            PrintSqlError(query.lastError());
        }

//        if (!query.exec("CREATE UNIQUE INDEX IF NOT EXISTS groupNameIndex ON groups (groupName)"))
//        {
//            PrintSqlError(query.lastError());
//        }
        db.close();
    }
}

qint64 GroupsTable::AddGroup(QString newGroupName)
{
    quint64 result = 0;
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec("SELECT * FROM groups WHERE groupName = '" + newGroupName + "'"))
        {
            if (query.next())
            {
                QMessageBox::information(NULL, "Group Name error", "Error: Cannot add group " + newGroupName + " because that group name already exists.", QMessageBox::Ok);
            }
            else
            {
                if (!query.exec("INSERT INTO groups (groupName) VALUES ('" + newGroupName + "')"))
                {
                    PrintSqlError(query.lastError());
                }
                else
                {
                    if (!query.lastInsertId().isNull() && query.lastInsertId().isValid())
                        result = query.lastInsertId().toLongLong();
                }
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

QHash<qint64, QString> GroupsTable::GetAllGroups()
{
    QHash<qint64, QString> groups;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.exec("SELECT * FROM groups"))
        {
            while (query.next())
            {
                QSqlRecord record = query.record();

                if (!record.isEmpty())
                {
                    QSqlField groupName = record.field("groupName");
                    QSqlField groupId = record.field("id");

                    if (!groupId.isNull() && groupId.isValid() &&
                            !groupName.isNull() && groupName.isValid())
                    {
                        qint64 id = groupId.value().toLongLong();
                        QString value = groupName.value().toString();
                        groups[id] = value;
                    }
                }
            }
        }
        else
        {
            PrintSqlError(query.lastError());
        }
        db.close();
    }

    return groups;
}
