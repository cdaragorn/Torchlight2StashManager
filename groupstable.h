#ifndef GROUPSTABLE_H
#define GROUPSTABLE_H

#include <QObject>
#include <sqlitetable.h>
#include <QList>

struct Group
{
    qint64 groupId;
    qint64 parentId;
    QString groupName;

    Group()
    {
        groupId = 0;
        parentId = 0;
        groupName = "";
    }
};

class GroupsTable : public SqliteTable
{
    Q_OBJECT
public:
    explicit GroupsTable(QString databaseName, QObject *parent = 0);

    qint64 AddGroup(QString newGroupName);
    Group GetGroup(qint64 groupId);
    QList<Group> GetAllGroups();
    QList<Group> GetAllTopLevelGroups();
    QList<Group> GetAllChildGroups(qint64 groupId);
    bool SetGroupParent(qint64 groupId, qint64 parentGroupId);
    
signals:
    
public slots:

private:
    Group ParseRecord(QSqlRecord inRecord);

    static const QString GroupId;
    static const QString GroupName;
    static const QString ParentGroupId;
    
};

#endif // GROUPSTABLE_H
