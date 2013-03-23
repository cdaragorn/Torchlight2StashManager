#ifndef SETTINGSTABPAGEFORM_H
#define SETTINGSTABPAGEFORM_H

#include <QWidget>
#include <optioncollection.h>
#include <optionkeys.h>

namespace Ui {
class SettingsTabPageForm;
}

class SettingsTabPageForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsTabPageForm(QWidget *parent = 0);
    ~SettingsTabPageForm();


    void Options(OptionCollection* inOptions)
    {
        mOptions = inOptions;

        LoadValues();
    }

signals:
    void torchlight2SharedStashFileChanged(QString fileLocation);
    void infiniteStashFolderChanged(QString folderLocation);

public slots:

private slots:
    void OnSetTorchlight2SharedStashFileClicked();
    void OnSetInfiniteStashFolderClicked();
    
private:
    Ui::SettingsTabPageForm *ui;

    OptionCollection* mOptions;

    void LoadValues();
};

#endif // SETTINGSTABPAGEFORM_H
