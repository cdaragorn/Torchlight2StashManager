#ifndef MANAGEGROUPSTABPAGE_H
#define MANAGEGROUPSTABPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <groupmanagertreeview.h>
#include <groupmanagerstandarditemmodel.h>
#include <groupstable.h>

class ManageGroupsTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit ManageGroupsTabPage(QWidget *parent = 0);

    void SetGroupNameLineEdit(QLineEdit* inLineEdit)
    {
        mGroupNameLineEdit = inLineEdit;
    }

    void SetAddGroupButton(QPushButton* inPushButton)
    {
        mAddGroupButton = inPushButton;

        if (mAddGroupButton != NULL)
        {
            connect(mAddGroupButton, SIGNAL(clicked()), this,
                    SLOT(OnAddGroupButtonClicked()));
        }
    }

    void SetGroupManagerTreeView(GroupManagerTreeView* inTreeView)
    {
        mGroupManagerTreeView = inTreeView;

        if (mGroupManagerTreeView != NULL)
        {
            mGroupManagerTreeView->setModel(mGroupManagerStandardItemModel);
        }
    }

    void SetGroupsTable(GroupsTable* inGroupsTable)
    {
        mGroupsTable = inGroupsTable;

        if (mGroupsTable != NULL)
        {
            LoadGroups();
        }
    }


    
signals:
    
public slots:

private slots:
    void OnAddGroupButtonClicked();

private:
    QLineEdit* mGroupNameLineEdit;
    QPushButton* mAddGroupButton;
    GroupManagerTreeView* mGroupManagerTreeView;
    GroupManagerStandardItemModel* mGroupManagerStandardItemModel;

    GroupsTable* mGroupsTable;

    void LoadGroups();
    
};

#endif // MANAGEGROUPSTABPAGE_H
