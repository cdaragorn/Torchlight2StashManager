#include "stashestable.h"

StashesTable::StashesTable(QString inDatabaseName, QObject *parent) :
    SqliteTable(inDatabaseName, parent)
{
    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (!query.exec("CREATE TABLE IF NOT EXISTS stashes (id INTEGER PRIMARY KEY, stashName TEXT, fileName TEXT)"))
        {
            PrintSqlError(query.lastError());
        }
        db.close();
    }
}
