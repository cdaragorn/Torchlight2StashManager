#ifndef GROUPSTABLE_H
#define GROUPSTABLE_H

#include <QObject>
#include <sqlitetable.h>
#include <QList>

class GroupsTable : public SqliteTable
{
    Q_OBJECT
public:
    explicit GroupsTable(QString databaseName, QObject *parent = 0);

    qint64 AddGroup(QString newGroupName);
    QHash<qint64, QString> GetAllGroups();
    
signals:
    
public slots:
    
};

#endif // GROUPSTABLE_H
