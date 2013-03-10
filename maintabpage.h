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

class MainTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit MainTabPage(QWidget *parent = 0);

    void SetTorchlight2SharedStashItemsListWidget(SharedStashListWidget* inListWidget)
    {
        mTorchlight2SharedStashItemsListWidget = inListWidget;
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
            QString torchlight2StashFile = mOptions.Get(OptionKeys::Torchlight2SharedStashFile);
            QString infiniteStashFolder = mOptions.Get(OptionKeys::StashManagerFolder);
        }

    }
    
signals:
    
public slots:

private:
    OptionCollection* mOptions;
    SharedStashListWidget* mTorchlight2SharedStashItemsListWidget;
    InfiniteStashItemsListWidget* mInfiniteStashItemsListWidget;
    QComboBox* mGroupsComboBox;
    GroupsTable* mGroupsTable;

    void FillGroupsComboBox();

    
};

#endif // MAINTABPAGE_H
