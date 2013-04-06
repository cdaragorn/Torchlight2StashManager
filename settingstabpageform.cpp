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

    connect(ui->Torchlight2SharedStashMaxNumberOfItemsSpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(OnMaxItemsPerStashTabValueChanged(qint32)));
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

        QString maxItemsPerStashTab = mOptions->Get(OptionKeys::MaxNumberOfItemsPerStashTab);

        if (maxItemsPerStashTab.isNull() || maxItemsPerStashTab.isEmpty())
        {
            maxItemsPerStashTab = "40";
            mOptions->Set(OptionKeys::MaxNumberOfItemsPerStashTab, maxItemsPerStashTab);
        }

        ui->Torchlight2SharedStashMaxNumberOfItemsSpinBox->setValue(maxItemsPerStashTab.toInt());
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
            emit torchlight2SharedStashFileChanged(file);
        }
    }
}


void SettingsTabPageForm::OnSetInfiniteStashFolderClicked()
{
    if (mOptions != NULL)
    {
        QDir infiniteStashFolderPath(mOptions->Get(OptionKeys::StashManagerFolder));
        QString folder = QDir::current().absolutePath();

        if (infiniteStashFolderPath.exists())
        {
            folder = QFileDialog::getExistingDirectory(this, "Choose folder", infiniteStashFolderPath.absolutePath());
        }
        else
        {
            folder = QFileDialog::getExistingDirectory(this, "Choose folder");
        }

        infiniteStashFolderPath.setPath(folder);

        if (infiniteStashFolderPath.exists())
        {
            ui->InfiniteStashFolderLineEdit->setText(folder);
            ui->InfiniteStashFolderLineEdit->setToolTip(folder);
            mOptions->Set(OptionKeys::StashManagerFolder, folder);
            emit infiniteStashFolderChanged(folder);
        }
    }
}

void SettingsTabPageForm::OnMaxItemsPerStashTabValueChanged(qint32 newValue)
{
    if (mOptions != NULL)
    {
        mOptions->Set(OptionKeys::MaxNumberOfItemsPerStashTab, QString::number(newValue));
        emit maxItemsPerStashTabChanged(newValue);
    }
}
