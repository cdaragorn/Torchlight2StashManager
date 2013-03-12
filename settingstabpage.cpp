#include "settingstabpage.h"

#include <QFileDialog>

SettingsTabPage::SettingsTabPage(QWidget *parent) :
    QWidget(parent)
{
    mOptions = NULL;
    mSetTorchlight2SharedStashFileButton = NULL;
    mSetInfiniteStashFolderButton = NULL;
    mTorchlight2SharedStashFileLineEdit = NULL;
    mInfiniteStashFolderLineEdit = NULL;
}

void SettingsTabPage::OnSetTorchlight2SharedStashFileClicked()
{
    QString currentSharedStashFile = mOptions->Get(OptionKeys::Torchlight2SharedStashFile);
    QString file;

    if (currentSharedStashFile.length() > 0)
    {
        file = QFileDialog::getOpenFileName(this, "Choose Torchlight 2 Shared Stash File", currentSharedStashFile);
    }
    else
    {
        file = QFileDialog::getOpenFileName(this, "Choose Torchlight 2 Shared Stash File");
    }

    if (!file.isNull() && !file.isEmpty())
    {
        mTorchlight2SharedStashFileLineEdit->setText(file);
        mTorchlight2SharedStashFileLineEdit->setToolTip(file);
        mOptions->Set(OptionKeys::Torchlight2SharedStashFile, file);
        torchlight2SharedStashFileChanged(file);
    }
}


void SettingsTabPage::OnSetInfiniteStashFolderClicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Choose folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        mInfiniteStashFolderLineEdit->setText(folder);
        mInfiniteStashFolderLineEdit->setToolTip(folder);
        mOptions->Set(OptionKeys::StashManagerFolder, folder);
        infiniteStashFolderChanged(folder);
    }
}
