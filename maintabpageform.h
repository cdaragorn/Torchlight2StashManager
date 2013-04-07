#ifndef MAINTABPAGEFORM_H
#define MAINTABPAGEFORM_H

#include <QWidget>
#include <optioncollection.h>
#include <optionkeys.h>
#include <torchlight2stash.h>
#include <QListWidgetItem>
#include <QStandardItem>
#include <infinitestashstandarditemmodel.h>
#include <settingstabpageform.h>
#include <stashitemstable.h>

namespace Ui {
class MainTabPageForm;
}

class MainTabPageForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainTabPageForm(QWidget *parent = 0);
    ~MainTabPageForm();

    Torchlight2Stash* GetTorchlight2Stash() { return mTorchlight2Stash; }


    void SetSettingsTabPage(SettingsTabPageForm* inSettingsTabPage)
    {
        mSettingsTabPage = inSettingsTabPage;

        if (mSettingsTabPage != NULL)
        {
            connect(mSettingsTabPage, SIGNAL(torchlight2SharedStashFileChanged(QString)), this,
                    SLOT(OnTorchlight2SharedStashFileChanged(QString)));

            connect(mSettingsTabPage, SIGNAL(maxItemsPerStashTabChanged(qint32)), this,
                    SLOT(OnMaximumNumberOfItemsPerTabChanged(qint32)));
        }
    }

    void SetInfiniteStashStandardItemModel(InfiniteStashStandardItemModel* inModel);

    void SetStashItemsTable(StashItemsTable* inTable)
    {
        mStashItemsTable = inTable;
    }

    void Options(OptionCollection* inOptions)
    {
        mOptions = inOptions;

        if (mOptions != NULL)
        {
            QString torchlight2StashFile = mOptions->Get(OptionKeys::Torchlight2SharedStashFile);
            QString infiniteStashFolder = mOptions->Get(OptionKeys::StashManagerFolder);

            FillSharedStashList(torchlight2StashFile);
        }
    }

signals:

private slots:
    void OnTorchlight2SharedStashItemAdded(QListWidgetItem* item);
    void OnTorchlight2SharedStashItemsRemoved(QList<QListWidgetItem*> item);
    void OnTorchlight2SharedStashFileChanged(QString fileLocation);
    void OnMaximumNumberOfItemsPerTabChanged(qint32 newMax);
//    void OnItemDroppedOnInfiniteStash(QDropEvent* event);
    void OnItemDroppedOnTorchlight2SharedStash(QDropEvent* event);
    void OnItemDraggedToTorchlight2SharedStash(QDragEnterEvent* event);
    void OnTorchlight2StashItemActivated(int row);
    
private:
    Ui::MainTabPageForm *ui;

    OptionCollection* mOptions;
    StashItemsTable* mStashItemsTable;
    bool mIsLoading;

    InfiniteStashStandardItemModel* mInfiniteStashTreeViewModel;

    SettingsTabPageForm* mSettingsTabPage;

    Torchlight2Stash* mTorchlight2Stash;


    void FillSharedStashList(QString inTorchlight2SharedStashFile);

};

#endif // MAINTABPAGEFORM_H
