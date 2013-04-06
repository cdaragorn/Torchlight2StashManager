#include "groupstable.h"

const QString GroupsTable::GroupId = "groupId";
const QString GroupsTable::GroupName = "groupName";
const QString GroupsTable::ParentGroupId = "parentGroupId";

GroupsTable::GroupsTable(QString databaseName)
{
    DatabasePath(databaseName);
}

void GroupsTable::DatabasePath(QString filePath)
{
    DatabaseName(filePath);

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

//        if (!groupId.isNull() && groupId.isValid() &&
//                !groupName.isNull() && groupName.isValid() &&
//                !parentId.isNull() && parentId.isValid() &&
//                !orderNumber.isNull() && orderNumber.isValid())
//        {
            result.groupId = groupId.value().toLongLong();
            result.groupName = groupName.value().toString();
            result.parentId = parentId.value().toLongLong();
//        }
    }

    return result;
}


qint64 GroupsTable::AddGroup(Group groupToAdd)
{
    qint64 result = 0;
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("INSERT INTO groups (" + GroupName + ", " + ParentGroupId +
                          ") VALUES (:newGroupName, :parentGroupId)"))
        {
            query.bindValue(":newGroupName", groupToAdd.groupName);
            query.bindValue(":parentGroupId", groupToAdd.parentId);

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

        if (query.exec("SELECT * FROM groups WHERE " + ParentGroupId + " = 0 ORDER BY " + GroupName))
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

        if (query.prepare("SELECT * FROM groups WHERE " + ParentGroupId +
                          " = :groupId ORDER BY " + GroupName))
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

bool GroupsTable::EditGroup(Group editedGroup)
{
    bool result = false;

    if (editedGroup.groupId > 0)
    {
        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("UPDATE groups SET " + ParentGroupId + " = :parentGroupId, " +
                              GroupName + " = :groupName WHERE " +
                              GroupId + " = :groupId"))
            {
                query.bindValue(":parentGroupId", editedGroup.parentId);
                query.bindValue(":groupName", editedGroup.groupName);
                query.bindValue(":groupId", editedGroup.groupId);

                result = query.exec();
            }

            if (!result)
            {
                PrintSqlError(query.lastError());
            }

            db.close();
        }
    }

    return result;
}

bool GroupsTable::SetGroupParent(qint64 groupId, qint64 parentGroupId)
{
    bool result = false;
    Group groupBeingSetAsParent = GetGroup(parentGroupId);
    Group groupBeingMoved = GetGroup(groupId);

    if (groupBeingMoved.groupId > 0 && groupBeingMoved.groupName != "")
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

/*
 *  Deletes the passed in group id from the table
 *
 */
bool GroupsTable::DeleteGroup(qint64 groupId)
{
    bool result = false;

    if (groupId > 0)
    {
        QSqlDatabase db = GetDatabase();

        if (db.open())
        {
            QSqlQuery query(db);

            if (query.prepare("DELETE FROM groups WHERE " + GroupId + " = :groupId"))
            {
                query.bindValue(":groupId", groupId);

                result = query.exec();

                if (!result)
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
