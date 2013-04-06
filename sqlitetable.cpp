#include "sqlitetable.h"

#include <sstream>


const QString SqliteTable::SqlConnectionName = "Torchlight2SharedStashSqliteConnection";

SqliteTable::SqliteTable()
{

}

QSqlDatabase SqliteTable::GetDatabase()
{
    QSqlDatabase db = QSqlDatabase::database(SqlConnectionName, false);

    if(!db.isValid())
    {
        db = QSqlDatabase::addDatabase("QSQLITE", SqlConnectionName);
    }

    db.setDatabaseName(mDatabaseName);

    return db;
}

SqliteTable::~SqliteTable()
{

}

void SqliteTable::DatabaseName(QString filePath)
{
    mDatabaseName = filePath;
}

void SqliteTable::PrintSqlError(QSqlError error)
{
    QMessageBox::information(NULL, "Sql Error", error.text(), QMessageBox::Ok);
}
