#ifndef MANAGEGROUPSTABPAGEFORM_H
#define MANAGEGROUPSTABPAGEFORM_H

#include <QWidget>
#include <groupstable.h>
#include <infinitestashstandarditemmodel.h>

namespace Ui {
class ManageGroupsTabPageForm;
}

class ManageGroupsTabPageForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ManageGroupsTabPageForm(QWidget *parent = 0);
    ~ManageGroupsTabPageForm();


    void SetGroupManagerStandardItemModel(InfiniteStashStandardItemModel* inStandardItemModel);


    void SetGroupsTable(GroupsTable* inGroupsTable)
    {
        mGroupsTable = inGroupsTable;

        if (mGroupsTable != NULL)
        {
//            LoadGroups();
        }
    }

signals:

public slots:

private slots:
    void OnAddGroupButtonClicked();
    
private:
    Ui::ManageGroupsTabPageForm *ui;

    InfiniteStashStandardItemModel* mGroupManagerStandardItemModel;

    GroupsTable* mGroupsTable;
};

#endif // MANAGEGROUPSTABPAGEFORM_H
