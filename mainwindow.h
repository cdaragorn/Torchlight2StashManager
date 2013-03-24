#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QStandardItem>
#include "optioncollection.h"
#include <groupstable.h>
#include <stashitemstable.h>
#include <infinitestashstandarditemmodel.h>


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
//    virtual void resizeEvent(QResizeEvent * inEvent);

private:
    Ui::MainWindow *ui;
    OptionCollection mOptions;
    GroupsTable* mGroupsTable;
    StashItemsTable* mStashItemsTable;


    InfiniteStashStandardItemModel* mInfiniteStashModel;

//    void FillGroupsComboBox();
    void LoadOptions();
    void LoadGroups();

    void LoadGroupChildren(Group inGroup, QStandardItem* parentGroup);
    QStandardItem* AddGroupToModel(Group inGroup, QStandardItem* parentItem);
    void AddItemToModel(StashItem item, QStandardItem* parentGroup);

private slots:
    void OnBoogeyClicked();
    void OnCreateNewStashClicked();
    void OnOptionsChanged();
    void OnInfiniteStashModelItemChanged(QStandardItem* inItem);

};

#endif // MAINWINDOW_H
