#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWhatsThis>
#include <addgroupdialog.h>
#include <QList>
#include <QHash>
#include <QFileDialog>

#include <torchlight2stashconverter.h>
#include <optionkeys.h>
#include <torchlight2stash.h>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mGroupsTable("database.sqlite")
{
    ui->setupUi(this);

    ui->SettingsTab->SetTorchlight2SharedStashFileButton(ui->SettingsTabTorchlight2FolderButton);
    ui->SettingsTab->SetTorchlight2SharedStashFileLineEdit(ui->SettingsTabTorchlight2FolderLineEdit);
    ui->SettingsTab->SetInfiniteStashFolderButton(ui->SettingsTabStashesFolderButton);
    ui->SettingsTab->SetInfiniteStashFolderLineEdit(ui->SettingsTabStashesFolderLineEdit);

    ui->MainTab->SetTorchlight2SharedStashItemsListWidget(ui->MainTabSharedStashItemsListWidget);
    ui->MainTab->SetInfiniteStashItemsListWidget(ui->MainTabInfiniteStashItemsListWidget);
    ui->MainTab->SetGroupsTable(&mGroupsTable);
    ui->MainTab->SetNumberOfItemsInSharedStashLabel(ui->MainTabNumberOfItemsInSharedStashLabel);
    ui->MainTab->SetGroupsComboBox(ui->MainTabGroupsComboBox);

//    ui->MainTabControlWidget->move(0, 0);
//    QLayout* layout = ui->MainTab->layout();
//    QPushButton* button = new QPushButton(ui->MainTab);
//    button->setText("Boogey");
//    button->move(180, 50);
//    button->resize(80, 30);
//    connect(button, SIGNAL(pressed()), this, SLOT(OnBoogeyClicked()));

    connect(ui->actionCreateNewStash, SIGNAL(triggered()), this, SLOT(OnCreateNewStashClicked()));

    ui->MainToolBar->addAction(QWhatsThis::createAction());

    connect(ui->AddGroupButton, SIGNAL(pressed()), this, SLOT(OnAddGroup()));

    connect(&mOptions, SIGNAL(OptionsChanged(QString, QString)), this, SLOT(OnOptionsChanged()));

    connect(ui->SettingsTabTestFileDescramblerButton, SIGNAL(clicked()), this, SLOT(OnTestFileDescramblerClicked()));

//    FillGroupsComboBox();

//    layout->addWidget(button);
//    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
//    layout->addWidget(button);
//    ui->MainTab->layout()->addWidget(button);
//    ui->MainTab->addAction(button);
//    QListWidgetItem* item = new QListWidgetItem(ui->SelectListWidget);
//    item->setText("Button");
//    QIcon* icon = new QIcon("://images/solid_iron_chest.png");

//    item->setIcon(*icon);

    LoadOptions();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadOptions()
{
    QDomDocument doc;

    QFile optionsFile("options.xml");

    if (optionsFile.open(QIODevice::ReadOnly))
    {
        doc.setContent(&optionsFile);

        mOptions.Load(doc.documentElement());

        ui->SettingsTab->Options(&mOptions);
        ui->MainTab->Options(&mOptions);
    }

}

//void MainWindow::resizeEvent(QResizeEvent * inEvent)
//{
//    QMainWindow::resizeEvent(inEvent);
//    int h = height();
//    int w = width();

//    ui->MainTabControlWidget->resize(w, h - 40 - ui->MainToolBar->height());
//}

void MainWindow::OnBoogeyClicked()
{
    cerr << "boogey!" << endl;
}

void MainWindow::OnCreateNewStashClicked()
{
    cerr << "create new stash!" << endl;
}

void MainWindow::OnAddGroup()
{
    AddGroupDialog groupsDialog(this);

    if (groupsDialog.exec() == QDialog::Accepted)
    {
        if (groupsDialog.GroupName().length() > 0)
        {
            quint64 insertId = mGroupsTable.AddGroup(groupsDialog.GroupName());

            if (insertId > 0)
                ui->MainTabGroupsComboBox->addItem(groupsDialog.GroupName(), insertId);
        }
    }
}



void MainWindow::OnOptionsChanged()
{
    QFile fileName("options.xml");

    if (fileName.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter writer(&fileName);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(2);

        writer.writeStartDocument();
        writer.writeStartElement("options");
        mOptions.Save(writer);
        writer.writeEndElement();
        writer.writeEndDocument();

        fileName.close();
    }
}

void MainWindow::OnTestFileDescramblerClicked()
{
    QString torchlight2StashFile = mOptions.Get(OptionKeys::Torchlight2SharedStashFile);
    QString stashesFolder = mOptions.Get(OptionKeys::StashManagerFolder);

    QString decryptedFilePath;
//    QString reencryptedFilePath;

    if (stashesFolder.length() > 0)
    {
        decryptedFilePath = stashesFolder + "/decryptedStash.bin";

//        reencryptedFilePath = stashesFolder + "/re-encryptedStash.bin";
    }

    QByteArray decryptedBytes;

    QFile stashFile(torchlight2StashFile);

    if (stashFile.open(QIODevice::ReadOnly))
    {
        Torchlight2StashConverter::DescrambleFile(stashFile.readAll(), decryptedBytes);
        stashFile.close();

//        QFile decryptedFile(decryptedFilePath);

//        if (decryptedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
//        {
//            decryptedFile.write(decryptedBytes);
//            decryptedFile.close();
//        }

//        QFile reencryptedFile(reencryptedFilePath);

//        if (reencryptedFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
//        {
//            QByteArray outputBuffer;
//            Torchlight2StashConverter::ScrambleFile(decryptedBytes, outputBuffer);

//            reencryptedFile.write(outputBuffer);
//            reencryptedFile.close();
//        }

        Torchlight2Stash stash(decryptedBytes);

        QHash<QString, QByteArray*> itemsInStash = stash.StashItems();
        QList<QString> keys = itemsInStash.keys();

        for (int i = 0; i < keys.count(); ++i)
        {
            QListWidgetItem* item = new QListWidgetItem(keys[i]);
            QVariant itemData(*itemsInStash[keys[i]]);
            item->setData(Qt::UserRole, itemData);
            ui->MainTabSharedStashItemsListWidget->addItem(item);
        }
    }


}
