#ifndef GROUPSTABLE_H
#define GROUPSTABLE_H

#include <QObject>
#include <sqlitetable.h>
#include <QList>

struct Group;

class GroupsTable : public SqliteTable
{
public:
    explicit GroupsTable(QString databaseName);

    void DatabasePath(QString filePath);

    qint64 AddGroup(Group groupToAdd);
    Group GetGroup(qint64 groupId);
    QList<Group> GetAllGroups();
    QList<Group> GetAllTopLevelGroups();
    QList<Group> GetAllChildGroups(qint64 groupId);
    bool SetGroupParent(qint64 groupId, qint64 parentGroupId);
    bool EditGroup(Group editedGroup);
    bool DeleteGroup(qint64 groupId);
    
signals:
    
public slots:

private:
    Group ParseRecord(QSqlRecord inRecord);

    static const QString GroupId;
    static const QString GroupName;
    static const QString ParentGroupId;
    
};

struct Group
{
public:
    Group()
    {
        groupId = 0;
        parentId = 0;
        groupName = "";
    }

    qint64 groupId;
    qint64 parentId;
    QString groupName;

};

#endif // GROUPSTABLE_H
