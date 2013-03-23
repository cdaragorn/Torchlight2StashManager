#include "settingstabpageform.h"
#include "ui_settingstabpageform.h"

#include <QFileDialog>

SettingsTabPageForm::SettingsTabPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTabPageForm)
{
    ui->setupUi(this);

    mOptions = NULL;

    connect(ui->InfiniteStashFolderPushButton, SIGNAL(pressed()), this,
            SLOT(OnSetInfiniteStashFolderClicked()));

    connect(ui->Torchlight2SharedStashFolderPushButton, SIGNAL(pressed()), this,
            SLOT(OnSetTorchlight2SharedStashFileClicked()));
}

SettingsTabPageForm::~SettingsTabPageForm()
{
    delete ui;
}

void SettingsTabPageForm::LoadValues()
{
    if (mOptions != NULL)
    {
        ui->InfiniteStashFolderLineEdit->setText(mOptions->Get(OptionKeys::StashManagerFolder));
        ui->InfiniteStashFolderLineEdit->setToolTip(ui->InfiniteStashFolderLineEdit->text());
        ui->Torchlight2SharedStashFolderLineEdit->setText(mOptions->Get(OptionKeys::Torchlight2SharedStashFile));
        ui->Torchlight2SharedStashFolderLineEdit->setToolTip(ui->Torchlight2SharedStashFolderLineEdit->text());
    }
}

void SettingsTabPageForm::OnSetTorchlight2SharedStashFileClicked()
{
    if (mOptions != NULL)
    {
        QString currentSharedStashFile = mOptions->Get(OptionKeys::Torchlight2SharedStashFile);
        QString file;

        QString openFileMessage = "Choose Torchlight 2 Shared Stash File";

        if (currentSharedStashFile.length() > 0)
        {
            file = QFileDialog::getOpenFileName(this, openFileMessage, currentSharedStashFile);
        }
        else
        {
            file = QFileDialog::getOpenFileName(this, openFileMessage);
        }

        if (!file.isNull() && !file.isEmpty())
        {
            ui->Torchlight2SharedStashFolderLineEdit->setText(file);
            ui->Torchlight2SharedStashFolderLineEdit->setToolTip(file);
            mOptions->Set(OptionKeys::Torchlight2SharedStashFile, file);
            torchlight2SharedStashFileChanged(file);
        }
    }
}


void SettingsTabPageForm::OnSetInfiniteStashFolderClicked()
{
    if (mOptions != NULL)
    {
        QString folder = QFileDialog::getExistingDirectory(this, "Choose folder");

        if (!folder.isNull() && !folder.isEmpty())
        {
            ui->InfiniteStashFolderLineEdit->setText(folder);
            ui->InfiniteStashFolderLineEdit->setToolTip(folder);
            mOptions->Set(OptionKeys::StashManagerFolder, folder);
            infiniteStashFolderChanged(folder);
        }
    }
}
