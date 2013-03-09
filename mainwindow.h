#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include "optioncollection.h"
#include <groupstable.h>


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
    GroupsTable mGroupsTable;

    void FillGroupsComboBox();
    void LoadUIOptions();

private slots:
    void OnBoogeyClicked();
    void OnCreateNewStashClicked();
    void OnAddGroup();
    void OnSetTorchlight2FolderClicked();
    void OnSetStashesFolderClicked();
    void OnOptionsChanged();
    void OnTestFileDescramblerClicked();

};

#endif // MAINWINDOW_H
