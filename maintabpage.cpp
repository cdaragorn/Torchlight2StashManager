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
}


void MainTabPage::FillGroupsComboBox()
{
    if (mGroupsComboBox != NULL)
    {
        mGroupsComboBox->clear();
        mGroupsComboBox->addItem("All Stashes");
        mGroupsComboBox->addItem("Stashes not in a group");
        mGroupsComboBox->addItem("Active Stash");
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
