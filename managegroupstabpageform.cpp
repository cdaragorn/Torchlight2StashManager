#include "managegroupstabpageform.h"
#include "ui_managegroupstabpageform.h"

#include <QStandardItem>

ManageGroupsTabPageForm::ManageGroupsTabPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageGroupsTabPageForm)
{
    ui->setupUi(this);

    mGroupManagerStandardItemModel = NULL;
    mGroupsTable = NULL;

    connect(ui->AddGroupPushButton, SIGNAL(clicked()), this, SLOT(OnAddGroupButtonClicked()));

    connect(ui->GroupNameLineEdit, SIGNAL(returnPressed()), this, SLOT(OnAddGroupButtonClicked()));

    connect(ui->GroupsTreeView, SIGNAL(groupDeleted(qint64)), this,
            SLOT(OnGroupBeingDeleted(qint64)));
}

ManageGroupsTabPageForm::~ManageGroupsTabPageForm()
{
    delete ui;
}

void ManageGroupsTabPageForm::SetGroupManagerStandardItemModel(InfiniteStashStandardItemModel* inStandardItemModel)
{
    mGroupManagerStandardItemModel = inStandardItemModel;

    if (mGroupManagerStandardItemModel != NULL)
    {
        ui->GroupsTreeView->setModel(mGroupManagerStandardItemModel);
    }
}

void ManageGroupsTabPageForm::OnAddGroupButtonClicked()
{
    QString newGroupName = ui->GroupNameLineEdit->text();

    if (newGroupName.length() > 0)
    {
        Group groupToAdd;
        groupToAdd.groupName = newGroupName;
        qint64 newGroupId = mGroupsTable->AddGroup(groupToAdd);

        if (newGroupId > 0)
        {
            QVariant id(newGroupId);
            QStandardItem* newGroupItem = new QStandardItem(newGroupName);
            newGroupItem->setData(id, Qt::UserRole);
            newGroupItem->setData(InfiniteStashStandardItemModel::Group, Qt::UserRole + 1);
            QIcon icon("://images/Open-Folder.png");
            newGroupItem->setIcon(icon);
            mGroupManagerStandardItemModel->appendRow(newGroupItem);
            ui->GroupNameLineEdit->clear();
        }
    }
}

void ManageGroupsTabPageForm::OnGroupBeingDeleted(qint64 inGroup)
{
    if (mGroupsTable != NULL && inGroup > 0)
    {
        mGroupsTable->DeleteGroup(inGroup);
    }
}
