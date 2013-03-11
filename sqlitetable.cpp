#include "sqlitetable.h"

#include <sstream>


const QString SqliteTable::SqlConnectionName = "Torchlight2SharedStashSqliteConnection";

SqliteTable::SqliteTable(QString databaseName, QObject *parent) :
    QObject(parent)
{
    mDatabaseName = databaseName;
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

void SqliteTable::PrintSqlError(QSqlError error)
{
    QMessageBox::information(NULL, "Sql Error", error.text(), QMessageBox::Ok);
}
