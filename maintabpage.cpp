#include "maintabpage.h"

#include <QDebug>
#include <QStandardItemModel>
#include <QMimeData>
#include <QVariant>

MainTabPage::MainTabPage(QWidget *parent) :
    QWidget(parent)
{
    mOptions = NULL;
    mTorchlight2SharedStashItemsListWidget = NULL;
    mGroupsComboBox = NULL;
    mNumberOfItemsInSharedStashLabel = NULL;
    mGroupsTable = NULL;
    mTorchlight2Stash = NULL;
    mSettingsTabPage = NULL;
    mInfiniteStashTreeView = NULL;
    mInfiniteStashTreeViewModel = NULL;

    mIsLoading = false;
}

MainTabPage::~MainTabPage()
{
    if (mTorchlight2Stash != NULL)
        delete mTorchlight2Stash;
}


void MainTabPage::FillGroupsComboBox()
{
    if (mGroupsComboBox != NULL)
    {
        mGroupsComboBox->clear();
        mGroupsComboBox->addItem("All Items");
        mGroupsComboBox->addItem("Items not in a group");
        mGroupsComboBox->insertSeparator(3);

        if (mGroupsTable != NULL)
        {
            QList<Group> groups = mGroupsTable->GetAllGroups();
            QList<Group>::const_iterator it;

            for (it = groups.constBegin(); it != groups.constEnd(); ++it)
            {
                Group nextGroup = *it;
                mGroupsComboBox->addItem(nextGroup.groupName, nextGroup.groupId);
//                mGroupsComboBox->addItem(it.value(), it.key());
            }
        }
    }
}


void MainTabPage::FillSharedStashList(QString fileLocation)
{
    if (mTorchlight2SharedStashItemsListWidget != NULL)
    {
        mIsLoading = true;

        mTorchlight2SharedStashItemsListWidget->clear();

        if (mTorchlight2Stash != NULL)
            delete mTorchlight2Stash;

        QByteArray decryptedBytes;

        QFile stashFile(fileLocation);

        if (stashFile.open(QIODevice::ReadOnly))
        {
            Torchlight2StashConverter::DescrambleFile(stashFile.readAll(), decryptedBytes);
            stashFile.close();

            mTorchlight2Stash = new Torchlight2Stash(decryptedBytes);

            QList<Torchlight2Item> itemsInStash = mTorchlight2Stash->StashItems();
    //        QList<QString> keys = itemsInStash.keys();

            for (int i = 0; i < itemsInStash.count(); ++i)
            {
                QListWidgetItem* item = new QListWidgetItem(itemsInStash[i].Name());
//                item->setFlags(item->flags() ^ Qt::ItemIsDropEnabled);

                QVariant itemData = QVariant::fromValue(itemsInStash[i].Bytes());
    //            QVariant itemData(*itemsInStash[keys[i]]);
                item->setData(Qt::UserRole, itemData);
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
                mTorchlight2SharedStashItemsListWidget->addItem(item);
            }
        }

        mIsLoading = false;
    }
}

void MainTabPage::OnTorchlight2SharedStashItemAdded(QListWidgetItem* item)
{
    if (mNumberOfItemsInSharedStashLabel != NULL && mTorchlight2SharedStashItemsListWidget != NULL)
    {
        qint32 count = mTorchlight2SharedStashItemsListWidget->count();
        mNumberOfItemsInSharedStashLabel->setText(QString::number(count));
    }
}

void MainTabPage::OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*> items)
{
    if (mNumberOfItemsInSharedStashLabel != NULL && mTorchlight2SharedStashItemsListWidget != NULL)
    {
        qint32 count = mTorchlight2SharedStashItemsListWidget->count() - items.count();

        mNumberOfItemsInSharedStashLabel->setText(QString::number(count));
    }
}

void MainTabPage::OnTorchlight2SharedStashFileChanged(QString fileLocation)
{
    FillSharedStashList(fileLocation);
}

void MainTabPage::OnInfiniteStashFolderChanged(QString folderLocation)
{

}


void MainTabPage::OnItemDroppedOnInfiniteStash(QDropEvent* event)
{
    QStringList types = event->mimeData()->formats();
    QStandardItem* itemDroppedOn = mInfiniteStashTreeViewModel->itemFromIndex(mInfiniteStashTreeView->indexAt(event->pos()));

    if (itemDroppedOn != NULL)
    {
        for (int i = 0; i < itemDroppedOn->rowCount(); ++i)
        {
            QStandardItem* nextItem = itemDroppedOn->child(i);
            int what = 3;
        }
    }

    //need to provide an override in the item view, but this is how we can tell if the item(s) were actually dropped
    //on the parent  If it is not on the item (above or below it), then it will
    //add it to the item's parent
//    QAbstractItemView::DropIndicatorPosition p = mInfiniteStashTreeView->dropIndicatorPosition();
    //If this is false, then the item was added to the dropped on item's parent
    bool wasOnItem = mInfiniteStashTreeView->wasDropIndicatorOnItem();

    if (!wasOnItem)
    {
        itemDroppedOn = itemDroppedOn->parent();
    }

    if (event->source() == mTorchlight2SharedStashItemsListWidget)
    {
        QList<QListWidgetItem*> selectedItems = mTorchlight2SharedStashItemsListWidget->selectedItems();

        for (int i = 0; i < selectedItems.length(); ++i)
        {
            QListWidgetItem* currentItem = selectedItems[i];
            int o = 4;
        }

        //Find any children who have byte arrays stored in them and convert them
        QString byteArrayTypeName = QVariant::typeToName(QVariant::ByteArray);

        for (int i = 0; i < itemDroppedOn->rowCount(); ++i)
        {
            QStandardItem* nextChild = itemDroppedOn->child(i);

            if (byteArrayTypeName.compare(nextChild->data(Qt::UserRole).typeName()))
            {
                QString passed = "passed!";
            }
        }

        int w = 3;
    }
    else if (event->source() == mInfiniteStashTreeView)
    {
        QModelIndex index = mInfiniteStashTreeView->currentIndex();

        QStandardItem* nextItem = mInfiniteStashTreeViewModel->itemFromIndex(index);

//        for (int i = 0; i < selectedIndexes.length(); ++i)
//        {
//            QStandardItem* nextItem = mInfiniteStashTreeViewModel->itemFromIndex(selectedIndexes[i]);
//            int o = 3;
//        }
        int l = 10;
    }

    QByteArray modelList = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QDataStream inputStream(&modelList, QIODevice::ReadOnly);
    QByteArray outputBytes;
    QDataStream outputStream(&outputBytes, QIODevice::ReadWrite);

    while (!inputStream.atEnd())
    {
        int row;
        int col;
        QMap<int, QVariant> roleDataMap;
        inputStream >> row >> col >> roleDataMap;

        QListWidgetItem* testItem = mTorchlight2SharedStashItemsListWidget->item(row);

        for (int i = 0; i < itemDroppedOn->rowCount(); ++i)
        {
            QStandardItem* nextChild = itemDroppedOn->child(i);

            QString byteArrayTypeName = QVariant::typeToName(QVariant::ByteArray);

            if (byteArrayTypeName.compare(nextChild->data(Qt::UserRole).typeName()))
            {
                QString passed = "passed!";
            }

            if (nextChild == (QStandardItem*)testItem)
            {
                int w = 5;
            }
        }

        QModelIndex index = mInfiniteStashTreeViewModel->index(row, col);
        QStandardItem* test2 = mInfiniteStashTreeViewModel->itemFromIndex(index);

        int j = 4;

        QVariant userData = roleDataMap[Qt::UserRole];

        userData.setValue(5);
        roleDataMap[Qt::UserRole] = userData;

        outputStream << row << col << roleDataMap;

        for (int i = 0; i < roleDataMap.keys().length(); ++i)
        {
            QVariant something = roleDataMap[roleDataMap.keys()[i]];
            QVariant test = roleDataMap[Qt::UserRole];
            if (something.canConvert(QVariant::ByteArray))
            {
                QByteArray what = something.toByteArray();
                int f = 3;
            }
        }
    }

    outputStream.device()->reset();
    outputStream.device()->close();



//    while (!outputStream.atEnd())
//    {
//        int row;
//        int col;
//        QMap<int, QVariant> roleDataMap;
//        outputStream >> row >> col >> roleDataMap;


//        QVariant userData = roleDataMap[Qt::UserRole];
//        int j = 4;


//    }


}
