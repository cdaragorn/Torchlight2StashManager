#include "maintabpage.h"

MainTabPage::MainTabPage(QWidget *parent) :
    QWidget(parent)
{
    mOptions = NULL;
    mTorchlight2SharedStashItemsListWidget = NULL;
    mInfiniteStashItemsListWidget = NULL;
    mGroupsComboBox = NULL;
    mNumberOfItemsInSharedStashLabel = NULL;
    mGroupsTable = NULL;
    mTorchlight2Stash = NULL;
    mSettingsTabPage = NULL;

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
            QHash<qint64, QString> groups = mGroupsTable->GetAllGroups();
            QHash<qint64, QString>::const_iterator it;

            for (it = groups.constBegin(); it != groups.constEnd(); ++it)
            {
                mGroupsComboBox->addItem(it.value(), it.key());
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
                QVariant itemData = QVariant::fromValue(itemsInStash[i].Bytes());
    //            QVariant itemData(*itemsInStash[keys[i]]);
                item->setData(Qt::UserRole, itemData);
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
