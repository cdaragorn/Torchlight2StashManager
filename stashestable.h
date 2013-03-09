#ifndef STASHESTABLE_H
#define STASHESTABLE_H

#include <sqlitetable.h>

class StashesTable : public SqliteTable
{
    Q_OBJECT
public:
    explicit StashesTable(QString inDatabaseName, QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // STASHESTABLE_H
