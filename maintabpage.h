#ifndef MAINTABPAGE_H
#define MAINTABPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include <sharedstashlistwidget.h>
#include <infinitestashitemslistwidget.h>
#include <optioncollection.h>
#include <optionkeys.h>
#include <groupstable.h>
#include <QLabel>
#include <QList>

class MainTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit MainTabPage(QWidget *parent = 0);

    void SetTorchlight2SharedStashItemsListWidget(SharedStashListWidget* inListWidget)
    {
        mTorchlight2SharedStashItemsListWidget = inListWidget;

        if (mTorchlight2SharedStashItemsListWidget != NULL)
        {
            connect(mTorchlight2SharedStashItemsListWidget,
                    SIGNAL(itemAdded(QListWidgetItem*)), this,
                    SLOT(OnTorchlight2SharedStashItemAdded(QListWidgetItem*)));

            connect(mTorchlight2SharedStashItemsListWidget,
                    SIGNAL(itemsAboutToBeRemoved(QList<QListWidgetItem*>)), this,
                    SLOT(OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*>)));
        }
    }

    void SetInfiniteStashItemsListWidget(InfiniteStashItemsListWidget* inListWidget)
    {
        mInfiniteStashItemsListWidget = inListWidget;
    }

    void SetGroupsComboBox(QComboBox* inComboBox)
    {
        mGroupsComboBox = inComboBox;

        FillGroupsComboBox();
    }

    void SetNumberOfItemsInSharedStashLabel(QLabel* inLabel)
    {
        mNumberOfItemsInSharedStashLabel = inLabel;
    }

    void SetGroupsTable(GroupsTable* inGroupsTable)
    {
        mGroupsTable = inGroupsTable;

        FillGroupsComboBox();
    }

    void Options(OptionCollection* inOptions)
    {
        mOptions = inOptions;

        if (mOptions != NULL)
        {
            QString torchlight2StashFile = mOptions->Get(OptionKeys::Torchlight2SharedStashFile);
            QString infiniteStashFolder = mOptions->Get(OptionKeys::StashManagerFolder);
        }

    }
    
signals:
    
public slots:

private slots:
    void OnTorchlight2SharedStashItemAdded(QListWidgetItem* item);
    void OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*> item);

private:
    OptionCollection* mOptions;
    SharedStashListWidget* mTorchlight2SharedStashItemsListWidget;
    InfiniteStashItemsListWidget* mInfiniteStashItemsListWidget;
    QComboBox* mGroupsComboBox;
    QLabel* mNumberOfItemsInSharedStashLabel;
    GroupsTable* mGroupsTable;

    void FillGroupsComboBox();

    
};

#endif // MAINTABPAGE_H
