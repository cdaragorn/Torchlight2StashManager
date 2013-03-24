#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWhatsThis>
#include <addgroupdialog.h>
#include <QList>
#include <QHash>
#include <QFileDialog>

#include <torchlight2stashconverter.h>
#include <optionkeys.h>
#include <torchlight2stash.h>
#include <torchlight2item.h>
#include <QStandardItemModel>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mGroupsTable(new GroupsTable("database.sqlite")),
    mStashItemsTable(new StashItemsTable("database.sqlite"))
{
    ui->setupUi(this);

//    mGroupsTable = new GroupsTable("database.sqlite");

    ui->MainTabPage->SetSettingsTabPage(ui->SettingsTabPage);

    mInfiniteStashModel = new InfiniteStashStandardItemModel();

    connect(mInfiniteStashModel, SIGNAL(itemChanged(QStandardItem*)), this,
            SLOT(OnInfiniteStashModelItemChanged(QStandardItem*)));

    LoadGroups();

    ui->MainTabPage->SetInfiniteStashStandardItemModel(mInfiniteStashModel);

    ui->ManageGroupsTab->SetGroupsTable(mGroupsTable);
    ui->ManageGroupsTab->SetGroupManagerStandardItemModel(mInfiniteStashModel);

    connect(ui->actionCreateNewStash, SIGNAL(triggered()), this, SLOT(OnCreateNewStashClicked()));

    ui->MainToolBar->addAction(QWhatsThis::createAction());

    connect(&mOptions, SIGNAL(OptionsChanged(QString, QString)), this, SLOT(OnOptionsChanged()));

    LoadOptions();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mGroupsTable;
    delete mInfiniteStashModel;
}

void MainWindow::OnInfiniteStashModelItemChanged(QStandardItem* inItem)
{
    if (inItem != NULL)
    {
        QVariant itemType = inItem->data(Qt::UserRole + 1);

        if (!itemType.isNull())
        {
            qint32 type = itemType.toInt();
            if (type == InfiniteStashStandardItemModel::Group)
            {
                QVariant idValue = inItem->data(Qt::UserRole);
                QStandardItem* parentItem = inItem->parent();
                Group groupToEdit;
                groupToEdit.groupId = idValue.toLongLong();

                if (groupToEdit.groupId > 0)
                {
                    groupToEdit.groupName = inItem->data(Qt::DisplayRole).toString();

                    if (parentItem != NULL)
                    {
                        QVariant parentIdValue = parentItem->data(Qt::UserRole);

                        if (!parentIdValue.isNull())
                        {
                            groupToEdit.parentId = parentIdValue.toLongLong();
                        }

                    }

                    mGroupsTable->EditGroup(groupToEdit);
                }
            }
            else if (type == InfiniteStashStandardItemModel::StashItem)
            {
                QVariant userData = inItem->data(Qt::UserRole);

                if (userData.type() == QVariant::ByteArray)
                {
                    QVariant parentGroupId = inItem->parent()->data(Qt::UserRole);

                    StashItem item;
                    item.item = userData.toByteArray();
                    item.groupId = parentGroupId.toLongLong();

                    qint64 itemId = mStashItemsTable->AddStashItem(item);
                    inItem->setData(itemId, Qt::UserRole);

                    //this must be done last because it triggers this event again
                    inItem->setFlags(inItem->flags() & ~Qt::ItemIsDropEnabled);
                }
                else
                {
                    qint64 itemId = userData.toLongLong();

                    StashItem item = mStashItemsTable->GetStashItem(itemId);

                    qint64 oldGroupId = inItem->parent()->data(Qt::UserRole).toLongLong();

                    //sometimes changes occur that don't affect anything we need to change
                    //check to make sure we actually need to call the database
                    if (item.groupId != oldGroupId)
                    {
                        item.groupId = oldGroupId;

                        mStashItemsTable->UpdateGroup(item);
                    }
                }

            }
        }

    }
}

void MainWindow::LoadOptions()
{
    QDomDocument doc;

    QFile optionsFile("options.xml");

    if (optionsFile.open(QIODevice::ReadOnly))
    {
        doc.setContent(&optionsFile);

        mOptions.Load(doc.documentElement());

        ui->SettingsTabPage->Options(&mOptions);
        ui->MainTabPage->Options(&mOptions);
//        ui->MainTab->Options(&mOptions);
    }

}

void MainWindow::LoadGroups()
{
    mInfiniteStashModel->clear();
    QList<Group> rootItems = mGroupsTable->GetAllTopLevelGroups();

    for (int i = 0; i < rootItems.length(); ++i)
    {
        Group nextGroup = rootItems[i];

        QStandardItem* group = AddGroupToModel(nextGroup, NULL);
        LoadGroupChildren(nextGroup, group);
    }
}

void MainWindow::LoadGroupChildren(Group inGroup, QStandardItem* parentGroup)
{
    if (inGroup.groupId > 0)
    {
        QList<Group> childGroups = mGroupsTable->GetAllChildGroups(inGroup.groupId);

        for (int i = 0; i < childGroups.length(); ++i)
        {
            Group nextGroup = childGroups[i];

            QStandardItem* group = AddGroupToModel(nextGroup, parentGroup);
            LoadGroupChildren(nextGroup, group);
        }

        QList<StashItem> items = mStashItemsTable->GetItemsInGroup(inGroup.groupId);

        for (int j = 0; j < items.length(); ++j)
        {
            AddItemToModel(items[j], parentGroup);
        }
    }
}

QStandardItem* MainWindow::AddGroupToModel(Group inGroup, QStandardItem* parentItem)
{
    QStandardItem* nextItem = NULL;
    if (inGroup.groupId > 0)
    {
        nextItem = new QStandardItem(inGroup.groupName);
        nextItem->setData(inGroup.groupId, Qt::UserRole);
        nextItem->setData(InfiniteStashStandardItemModel::Group, Qt::UserRole + 1);
        QIcon icon("://images/Open-Folder.png");
        nextItem->setIcon(icon);

        if (parentItem == NULL)
        {
            mInfiniteStashModel->appendRow(nextItem);
        }
        else
        {
            parentItem->appendRow(nextItem);
        }
    }
    return nextItem;
}


void MainWindow::AddItemToModel(StashItem item, QStandardItem *parentGroup)
{
    if (parentGroup != NULL)
    {
        Torchlight2Item itemDetails(item.item);
        QStandardItem* nextItem = new QStandardItem();
        nextItem->setText(itemDetails.Name());
        nextItem->setData(item.itemId, Qt::UserRole);
        nextItem->setData(InfiniteStashStandardItemModel::StashItem, Qt::UserRole + 1);
        nextItem->setFlags(nextItem->flags() & ~Qt::ItemIsDropEnabled);
        parentGroup->appendRow(nextItem);
    }
}

//void MainWindow::resizeEvent(QResizeEvent * inEvent)
//{
//    QMainWindow::resizeEvent(inEvent);
//    int h = height();
//    int w = width();

//    ui->MainTabControlWidget->resize(w, h - 40 - ui->MainToolBar->height());
//}

void MainWindow::OnBoogeyClicked()
{
    cerr << "boogey!" << endl;
}

void MainWindow::OnCreateNewStashClicked()
{
    cerr << "create new stash!" << endl;
}



void MainWindow::OnOptionsChanged()
{
    QFile fileName("options.xml");

    if (fileName.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter writer(&fileName);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(2);

        writer.writeStartDocument();
        writer.writeStartElement("options");
        mOptions.Save(writer);
        writer.writeEndElement();
        writer.writeEndDocument();

        fileName.close();
    }
}

