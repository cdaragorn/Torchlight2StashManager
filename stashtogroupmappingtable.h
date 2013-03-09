#ifndef STASHTOGROUPMAPPINGTABLE_H
#define STASHTOGROUPMAPPINGTABLE_H

#include <sqlitetable.h>

class StashToGroupMappingTable : public SqliteTable
{
    Q_OBJECT
public:
    explicit StashToGroupMappingTable(QString inDatabaseName, QObject *parent = 0);

    void AddStashToGroup(qint64 stashId, qint64 inGroupId);
    void RemoveStashFromGroup(qint64 stashId, qint64 inGroupId);
    QList<qint64> GetAllStashesInGroup(qint64 inGroupId);
    QList<qint64> GetAllGroupsStashIsIn(qint64 inStashId);
    
signals:
    
public slots:
    
};

#endif // STASHTOGROUPMAPPINGTABLE_H
