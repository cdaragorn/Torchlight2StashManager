#include "stashtogroupmappingtable.h"

#include <QSqlQuery>

#include <sstream>

StashToGroupMappingTable::StashToGroupMappingTable(QString inDatabaseName, QObject *parent) :
    SqliteTable(inDatabaseName, parent)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (!query.exec("CREATE TABLE IF NOT EXISTS stashToGroupMapping (id INTEGER PRIMARY KEY, stashId INTEGER, groupId INTEGER)"))
        {
            PrintSqlError(query.lastError());
        }
        db.close();
    }
}

void StashToGroupMappingTable::AddStashToGroup(qint64 inStashId, qint64 inGroupId)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        std::stringstream stream;
        stream << "INSERT INTO stashToGroupMapping (stashId, groupId) VALUES (" <<
                  inStashId << ", " << inGroupId << ")";
        QSqlQuery query(db);
        if (!query.exec(stream.str().c_str()))
        {
            PrintSqlError(query.lastError());
        }
        db.close();
    }
}

void StashToGroupMappingTable::RemoveStashFromGroup(qint64 inStashId, qint64 inGroupId)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        std::stringstream stream;
        stream << "DELETE FROM stashToGroupMapping WHERE stashId = " <<
                  inStashId << " AND groupId = " << inGroupId;
        QSqlQuery query(db);
        if (!query.exec(stream.str().c_str()))
        {
            PrintSqlError(query.lastError());
        }
        db.close();
    }
}


QList<qint64> StashToGroupMappingTable::GetAllStashesInGroup(qint64 inGroupId)
{
    QList<qint64> stashes;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec("SELECT stashId FROM stashToGroupMapping WHERE groupId = " + inGroupId))
        {
            while(query.next())
            {
                QSqlRecord record = query.record();

                if (!record.isEmpty())
                {
                    QSqlField stashId = record.field("stashId");

                    if (!stashId.isNull() && stashId.isValid())
                    {
                        stashes.append(stashId.value().toLongLong());
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

    return stashes;
}

QList<qint64> StashToGroupMappingTable::GetAllGroupsStashIsIn(qint64 inStashId)
{
    QList<qint64> groups;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec("SELECT groupId FROM stashToGroupMapping WHERE stashId = " + inStashId))
        {
            while(query.next())
            {
                QSqlRecord record = query.record();

                if (!record.isEmpty())
                {
                    QSqlField groupId = record.field("groupId");

                    if (!groupId.isNull() && groupId.isValid())
                    {
                        groups.append(groupId.value().toLongLong());
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
