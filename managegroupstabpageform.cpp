#include "managegroupstabpageform.h"
#include "ui_managegroupstabpageform.h"

#include <QStandardItem>

ManageGroupsTabPageForm::ManageGroupsTabPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageGroupsTabPageForm)
{
    ui->setupUi(this);

    mGroupManagerStandardItemModel = NULL;

    connect(ui->AddGroupPushButton, SIGNAL(clicked()), this, SLOT(OnAddGroupButtonClicked()));
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
        qint64 newGroupId = mGroupsTable->AddGroup(newGroupName);

        if (newGroupId > 0)
        {
            QVariant id(newGroupId);
            QStandardItem* newGroupItem = new QStandardItem(newGroupName);
            newGroupItem->setData(id, Qt::UserRole);
            QIcon icon("://images/Open-Folder.png");
            newGroupItem->setIcon(icon);
            mGroupManagerStandardItemModel->appendRow(newGroupItem);
            ui->GroupNameLineEdit->clear();
        }
    }
}
