#ifndef SETTINGSTABPAGE_H
#define SETTINGSTABPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <optioncollection.h>
#include <optionkeys.h>

class SettingsTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsTabPage(QWidget *parent = 0);

    void SetTorchlight2SharedStashFileButton(QPushButton* inButton)
    {
        mSetTorchlight2SharedStashFileButton = inButton;

        if (mSetTorchlight2SharedStashFileButton != NULL)
            connect(mSetTorchlight2SharedStashFileButton, SIGNAL(pressed()), this,
                    SLOT(OnSetTorchlight2SharedStashFileClicked()));

    }

    void SetTorchlight2SharedStashFileLineEdit(QLineEdit* inLineEdit)
    {
        mTorchlight2SharedStashFileLineEdit = inLineEdit;
    }

    void SetInfiniteStashFolderButton(QPushButton* inButton)
    {
        mSetInfiniteStashFolderButton = inButton;

        if (mSetInfiniteStashFolderButton != NULL)
            connect(mSetInfiniteStashFolderButton, SIGNAL(pressed()), this, SLOT(OnSetInfiniteStashFolderClicked()));
    }

    void SetInfiniteStashFolderLineEdit(QLineEdit* inLineEdit)
    {
        mInfiniteStashFolderLineEdit = inLineEdit;
    }

    void Options(OptionCollection* inOptions)
    {
        mOptions = inOptions;

        if (mOptions != NULL)
        {
            if (mInfiniteStashFolderLineEdit != NULL)
                mInfiniteStashFolderLineEdit->setText(mOptions->Get(OptionKeys::StashManagerFolder));

            if (mTorchlight2SharedStashFileLineEdit != NULL)
                mTorchlight2SharedStashFileLineEdit->setText(mOptions->Get(OptionKeys::Torchlight2SharedStashFile));
        }
    }


    
signals:
    void torchlight2SharedStashFileChanged(QString fileLocation);
    void infiniteStashFolderChanged(QString folderLocation);
    
public slots:

private slots:
    void OnSetTorchlight2SharedStashFileClicked();
    void OnSetInfiniteStashFolderClicked();

private:
    OptionCollection* mOptions;
    QPushButton* mSetTorchlight2SharedStashFileButton;
    QPushButton* mSetInfiniteStashFolderButton;
    QLineEdit* mTorchlight2SharedStashFileLineEdit;
    QLineEdit* mInfiniteStashFolderLineEdit;
    
};

#endif // SETTINGSTABPAGE_H
