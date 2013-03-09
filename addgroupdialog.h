#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QDialog>

class AddGroupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddGroupDialog(QWidget *parent = 0);

    QString GroupName();
    
signals:
    
private slots:
    void OnGroupNameTextChanged(QString newText);

private:
    QString mGroupName;
    
};

#endif // ADDGROUPDIALOG_H
