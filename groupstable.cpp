#include "groupstable.h"

const QString GroupsTable::GroupId = "groupId";
const QString GroupsTable::GroupName = "groupName";
const QString GroupsTable::ParentGroupId = "parentGroupId";

GroupsTable::GroupsTable(QString databaseName, QObject *parent) :
    SqliteTable(databaseName, parent)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);
        if (!query.exec("CREATE TABLE IF NOT EXISTS groups (" +
                        GroupId + " INTEGER PRIMARY KEY, " +
                        ParentGroupId + " INTEGER NOT NULL default 0, " +
                        GroupName + " TEXT)"))
        {
            PrintSqlError(query.lastError());
        }
        else if (!query.exec("CREATE INDEX IF NOT EXISTS groupParentIdIndex ON groups (" + ParentGroupId + ")"))
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

Group GroupsTable::ParseRecord(QSqlRecord inRecord)
{
    Group result;

    if (!inRecord.isEmpty())
    {
        QSqlField groupName = inRecord.field(GroupName);
        QSqlField parentId = inRecord.field(ParentGroupId);
        QSqlField groupId = inRecord.field(GroupId);

        if (!groupId.isNull() && groupId.isValid() &&
                !groupName.isNull() && groupName.isValid() &&
                !parentId.isNull() && parentId.isValid())
        {
            result.groupId = groupId.value().toLongLong();
            result.groupName = groupName.value().toString();
            result.parentId = parentId.value().toLongLong();
        }
    }

    return result;
}

qint64 GroupsTable::AddGroup(QString newGroupName)
{
    qint64 result = 0;
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO groups (groupName) VALUES (:newGroupName)"))
        {
            query.bindValue(":newGroupName", newGroupName);

            if (!query.exec())
            {
                PrintSqlError(query.lastError());
            }
            else
            {
                if (!query.lastInsertId().isNull() && query.lastInsertId().isValid())
                    result = query.lastInsertId().toLongLong();
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

Group GroupsTable::GetGroup(qint64 groupId)
{
    Group result;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("SELECT * FROM groups WHERE " + GroupId + " = :groupId"))
        {
            query.bindValue(":groupId", groupId);

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

        db.close();
    }
    return result;
}

QList<Group> GroupsTable::GetAllGroups()
{
    QList<Group> groups;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.exec("SELECT * FROM groups"))
        {
            while (query.next())
            {
                Group nextGroup = ParseRecord(query.record());

                if (nextGroup.groupId > 0 && nextGroup.groupName != "")
                {
                    groups.append(nextGroup);
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

QList<Group> GroupsTable::GetAllTopLevelGroups()
{
    QList<Group> groups;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.exec("SELECT * FROM groups WHERE " + ParentGroupId + " = 0"))
        {
            while (query.next())
            {
                Group nextGroup = ParseRecord(query.record());

                if (nextGroup.groupId > 0 && nextGroup.groupName != "")
                {
                    groups.append(nextGroup);
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

QList<Group> GroupsTable::GetAllChildGroups(qint64 groupId)
{
    QList<Group> groups;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("SELECT * FROM groups WHERE " + ParentGroupId + " = :groupId"))
        {
            query.bindValue(":groupId", groupId);

            if (query.exec())
            {
                while (query.next())
                {
                    Group nextGroup = ParseRecord(query.record());

                    if (nextGroup.groupId > 0 && nextGroup.groupName != "")
                    {
                        groups.append(nextGroup);
                    }
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

    return groups;
}

bool GroupsTable::SetGroupParent(qint64 groupId, qint64 parentGroupId)
{
    bool result = false;
    Group groupBeingSetAsParent = GetGroup(parentGroupId);
    Group groupBeingMoved = GetGroup(groupId);

    if (groupBeingMoved.groupId > 0 && groupBeingMoved.groupName != "" &&
            groupBeingSetAsParent.groupId > 0 && groupBeingSetAsParent.groupName != "")
    {

        //If we are setting the groups parent as one of it's current children,
        //then we first need to set that child's parent to this group's parent
        if (groupBeingSetAsParent.parentId > 0 &&
                groupBeingSetAsParent.parentId == groupBeingMoved.groupId)
        {
            SetGroupParent(groupBeingSetAsParent.groupId, groupBeingMoved.parentId);
        }

        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("UPDATE groups SET " + ParentGroupId + " = :parentGroupId WHERE " + GroupId + " = :groupId"))
            {
                query.bindValue(":parentGroupId", parentGroupId);
                query.bindValue(":groupId", groupId);

                result = query.exec();

                if (!result)
                {
                    PrintSqlError(query.lastError());
                }
            }

            db.close();
        }
    }
    return result;
}
