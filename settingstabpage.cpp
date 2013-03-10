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

void SettingsTabPage::OnSetTorchlight2FolderClicked()
{
    QString folder = QFileDialog::getOpenFileName(this, "Choose Torchlight 2 Shared Stash File");

    if (!folder.isNull() && !folder.isEmpty())
    {
        mTorchlight2SharedStashFileLineEdit->setText(folder);
        mTorchlight2SharedStashFileLineEdit->setToolTip(folder);
        mOptions->Set(OptionKeys::Torchlight2SharedStashFile, folder);
    }
}


void SettingsTabPage::OnSetStashesFolderClicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Choose folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        mInfiniteStashFolderLineEdit->setText(folder);
        mInfiniteStashFolderLineEdit->setToolTip(folder);
        mOptions->Set(OptionKeys::StashManagerFolder, folder);
    }
}
