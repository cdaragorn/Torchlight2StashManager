#ifndef SQLITETABLE_H
#define SQLITETABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHash>

class SqliteTable
{
public:
    explicit SqliteTable();
    ~SqliteTable();
    
signals:
    
public slots:

protected:
    QSqlDatabase GetDatabase();
    void PrintSqlError(QSqlError);
    void DatabaseName(QString filePath);

private:
    QString mDatabaseName;

    static const QString SqlConnectionName;
    
};

#endif // SQLITETABLE_H
