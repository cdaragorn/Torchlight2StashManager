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

namespace Ui {
class MainTabPageForm;
}

class MainTabPageForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainTabPageForm(QWidget *parent = 0);
    ~MainTabPageForm();


    void SetSettingsTabPage(SettingsTabPageForm* inSettingsTabPage)
    {
        mSettingsTabPage = inSettingsTabPage;

        if (mSettingsTabPage != NULL)
        {
            connect(mSettingsTabPage, SIGNAL(torchlight2SharedStashFileChanged(QString)), this,
                    SLOT(OnTorchlight2SharedStashFileChanged(QString)));
        }
    }

    void SetInfiniteStashStandardItemModel(InfiniteStashStandardItemModel* inModel);

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
    void OnInfiniteStashFolderChanged(QString folderLocation);
    void OnItemDroppedOnInfiniteStash(QDropEvent* event);
    void OnItemDraggedToTorchlight2SharedStash(QDragEnterEvent* event);
    
private:
    Ui::MainTabPageForm *ui;

    OptionCollection* mOptions;
    bool mIsLoading;

    InfiniteStashStandardItemModel* mInfiniteStashTreeViewModel;

    SettingsTabPageForm* mSettingsTabPage;

    Torchlight2Stash* mTorchlight2Stash;


    void FillSharedStashList(QString inTorchlight2SharedStashFile);

};

#endif // MAINTABPAGEFORM_H
