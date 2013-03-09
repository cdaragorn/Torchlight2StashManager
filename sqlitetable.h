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

class SqliteTable : public QObject
{
    Q_OBJECT
public:
    explicit SqliteTable(QString databaseName, QObject *parent = 0);
    
signals:
    
public slots:

protected:
    QSqlDatabase GetDatabase();
    void PrintSqlError(QSqlError);

private:
    QString mDatabaseName;

    static const QString SqlConnectionName;
    
};

#endif // SQLITETABLE_H
