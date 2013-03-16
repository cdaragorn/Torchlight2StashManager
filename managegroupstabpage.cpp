#include "managegroupstabpage.h"

ManageGroupsTabPage::ManageGroupsTabPage(QWidget *parent) :
    QWidget(parent)
{
    mGroupManagerStandardItemModel = new GroupManagerStandardItemModel();

    mGroupNameLineEdit = NULL;
    mAddGroupButton = NULL;
    mGroupManagerTreeView = NULL;
    mGroupsTable = NULL;
}

void ManageGroupsTabPage::LoadGroups()
{
    QList<Group> rootItems = mGroupsTable->GetAllTopLevelGroups();

    for (int i = 0; i < rootItems.length(); ++i)
    {
        Group nextGroup = rootItems[i];

        if (nextGroup.groupId > 0)
        {
            QStandardItem* nextItem = new QStandardItem(nextGroup.groupName);
            nextItem->setData(nextGroup.groupId, Qt::UserRole);
            QIcon icon("://images/Open-Folder.png");
            nextItem->setIcon(icon);
            mGroupManagerStandardItemModel->appendRow(nextItem);
        }
    }
}

void ManageGroupsTabPage::OnAddGroupButtonClicked()
{
    if (mGroupNameLineEdit != NULL)
    {
        QString newGroupName = mGroupNameLineEdit->text();

        if (newGroupName.length() > 0)
        {
            qint64 newGroupId = mGroupsTable->AddGroup(newGroupName);

            if (newGroupId > 0)
            {
                QVariant id(newGroupId);
                QStandardItem* newGroupItem = new QStandardItem(newGroupName);
                newGroupItem->setData(id, Qt::UserRole);
                QIcon icon("://images/Open-Folder.png");
                newGroupItem->setIcon(icon);
                mGroupManagerStandardItemModel->appendRow(newGroupItem);
                mGroupNameLineEdit->clear();
            }
        }
    }
}
