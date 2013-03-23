#include "maintabpageform.h"
#include "ui_maintabpageform.h"

#include <torchlight2stashconverter.h>
#include <QMimeData>

MainTabPageForm::MainTabPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainTabPageForm)
{
    ui->setupUi(this);

    mTorchlight2Stash = NULL;

    mInfiniteStashTreeViewModel = NULL;



//    QStandardItem* parentItem = new QStandardItem("root");
//    parentItem->setDragEnabled(false);
//    parentItem->setEditable(false);
//    QIcon icon("://images/Open-Folder.png");
//    parentItem->setIcon(icon);
//    mInfiniteStashTreeViewModel->appendRow(parentItem);

//    for (int i = 0; i < 4; ++i)
//    {
//        QStandardItem* item = new QStandardItem(QString("item %0").arg(i));
//        item->setEditable(false);
//        item->setDragEnabled(false);
//        item->setIcon(icon);
//        parentItem->appendRow(item);
//        parentItem = item;
//    }

    mIsLoading = false;

    connect(ui->Torchlight2SharedStashListWidget,
            SIGNAL(itemAdded(QListWidgetItem*)), this,
            SLOT(OnTorchlight2SharedStashItemAdded(QListWidgetItem*)));

    connect(ui->Torchlight2SharedStashListWidget,
            SIGNAL(itemsAboutToBeRemoved(QList<QListWidgetItem*>)), this,
            SLOT(OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*>)));


    connect(ui->InfiniteStashItemsTreeView,
            SIGNAL(itemDropped(QDropEvent*)), this,
            SLOT(OnItemDroppedOnInfiniteStash(QDropEvent*)));

    connect(ui->Torchlight2SharedStashListWidget,
            SIGNAL(dragEntered(QDragEnterEvent*)),
            this,
            SLOT(OnItemDraggedToTorchlight2SharedStash(QDragEnterEvent*)));
}

MainTabPageForm::~MainTabPageForm()
{
    delete ui;
}

void MainTabPageForm::SetInfiniteStashStandardItemModel(InfiniteStashStandardItemModel* inModel)
{
    mInfiniteStashTreeViewModel = inModel;

    if (mInfiniteStashTreeViewModel != NULL)
    {
        ui->InfiniteStashItemsTreeView->setModel(mInfiniteStashTreeViewModel);
    }
}

void MainTabPageForm::FillSharedStashList(QString inTorchlight2SharedStashFile)
{

    mIsLoading = true;

    ui->Torchlight2SharedStashListWidget->clear();

    if (mTorchlight2Stash != NULL)
        delete mTorchlight2Stash;

    QByteArray decryptedBytes;

    QFile stashFile(inTorchlight2SharedStashFile);

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

            //first user role is the items data
            item->setData(Qt::UserRole, itemData);

            //second user role is a marker so we know that this is an item from the stash
            item->setData(Qt::UserRole + 1, InfiniteStashStandardItemModel::StashItem);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled);
            ui->Torchlight2SharedStashListWidget->addItem(item);
        }
    }

    mIsLoading = false;
}

void MainTabPageForm::OnTorchlight2SharedStashItemAdded(QListWidgetItem* item)
{
    qint32 count = ui->Torchlight2SharedStashListWidget->count();
    ui->SharedStashItemCountLabel->setText(QString::number(count));
}

void MainTabPageForm::OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*> items)
{
    qint32 count = ui->Torchlight2SharedStashListWidget->count() - items.count();

    ui->SharedStashItemCountLabel->setText(QString::number(count));
}

void MainTabPageForm::OnTorchlight2SharedStashFileChanged(QString fileLocation)
{
    FillSharedStashList(fileLocation);
}

void MainTabPageForm::OnInfiniteStashFolderChanged(QString folderLocation)
{

}


void MainTabPageForm::OnItemDroppedOnInfiniteStash(QDropEvent* event)
{
    QStringList types = event->mimeData()->formats();
    QStandardItem* itemDroppedOn = mInfiniteStashTreeViewModel->itemFromIndex(ui->InfiniteStashItemsTreeView->indexAt(event->pos()));

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
    bool wasOnItem = ui->InfiniteStashItemsTreeView->wasDropIndicatorOnItem();

    if (!wasOnItem)
    {
        itemDroppedOn = itemDroppedOn->parent();
    }

    if (event->source() == ui->Torchlight2SharedStashListWidget)
    {
        QList<QListWidgetItem*> selectedItems = ui->Torchlight2SharedStashListWidget->selectedItems();

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
    else if (event->source() == ui->InfiniteStashItemsTreeView)
    {
        QModelIndex index = ui->InfiniteStashItemsTreeView->currentIndex();

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

        QListWidgetItem* testItem = ui->Torchlight2SharedStashListWidget->item(row);

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


/*
 *  We capture this event in order to prevent groups from being dragged onto the
 *  Torchlight 2 stash list.
 *
 */
void MainTabPageForm::OnItemDraggedToTorchlight2SharedStash(QDragEnterEvent* event)
{

    if (event->source() == ui->InfiniteStashItemsTreeView)
    {
        QByteArray modelList = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream inputStream(&modelList, QIODevice::ReadOnly);

        while (!inputStream.atEnd())
        {
            int row;
            int col;
            QMap<int, QVariant> roleDataMap;
            inputStream >> row >> col >> roleDataMap;

            QVariant itemType = roleDataMap[Qt::UserRole + 1];

            if (itemType.toInt() == InfiniteStashStandardItemModel::Group)
            {
                event->ignore();
                event->setDropAction(Qt::IgnoreAction);
            }
        }
    }
}
