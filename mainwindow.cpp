#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWhatsThis>
#include <addgroupdialog.h>
#include <QList>
#include <QHash>
#include <QFileDialog>

#include <torchlight2stashconverter.h>
#include <optionkeys.h>

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mGroupsTable("database.sqlite")
{
    ui->setupUi(this);
    ui->MainTabControlWidget->move(0, 0);
//    QLayout* layout = ui->MainTab->layout();
//    QPushButton* button = new QPushButton(ui->MainTab);
//    button->setText("Boogey");
//    button->move(180, 50);
//    button->resize(80, 30);
//    connect(button, SIGNAL(pressed()), this, SLOT(OnBoogeyClicked()));

    connect(ui->actionCreateNewStash, SIGNAL(triggered()), this, SLOT(OnCreateNewStashClicked()));

    ui->MainToolBar->addAction(QWhatsThis::createAction());

    connect(ui->AddGroupButton, SIGNAL(pressed()), this, SLOT(OnAddGroup()));

    connect(ui->SettingsTabTorchlight2FolderButton, SIGNAL(pressed()), this,
            SLOT(OnSetTorchlight2FolderClicked()));

    connect(ui->SettingsTabStashesFolderButton, SIGNAL(pressed()), this, SLOT(OnSetStashesFolderClicked()));

    connect(&mOptions, SIGNAL(OptionsChanged(QString, QString)), this, SLOT(OnOptionsChanged()));

    connect(ui->SettingsTabTestFileDescramblerButton, SIGNAL(clicked()), this, SLOT(OnTestFileDescramblerClicked()));

    FillGroupsComboBox();

//    layout->addWidget(button);
//    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
//    layout->addWidget(button);
//    ui->MainTab->layout()->addWidget(button);
//    ui->MainTab->addAction(button);
    QListWidgetItem* item = new QListWidgetItem(ui->SelectListWidget);
    item->setText("Button");
    QIcon* icon = new QIcon("://images/solid_iron_chest.png");

    item->setIcon(*icon);

    LoadUIOptions();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadUIOptions()
{
    QDomDocument doc;

    QFile optionsFile("options.xml");

    if (optionsFile.open(QIODevice::ReadOnly))
    {
        doc.setContent(&optionsFile);

        mOptions.Load(doc.documentElement());
        ui->SettingsTabStashesFolderLineEdit->setText(mOptions[OptionKeys::StashManagerFolder]);
        ui->SettingsTabTorchlight2FolderLineEdit->setText(mOptions[OptionKeys::Torchlight2SharedStashFile]);
    }

//    mOptions.Load();

}

void MainWindow::FillGroupsComboBox()
{
    QHash<qint64, QString> groups = mGroupsTable.GetAllGroups();
    QHash<qint64, QString>::const_iterator it;

    ui->GroupsComboBox->addItem("All Stashes");
    ui->GroupsComboBox->addItem("Stashes not in a group");
    ui->GroupsComboBox->addItem("Active Stash");
    ui->GroupsComboBox->insertSeparator(3);

    for (it = groups.constBegin(); it != groups.constEnd(); ++it)
    {
        ui->GroupsComboBox->addItem(it.value(), it.key());
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
                ui->GroupsComboBox->addItem(groupsDialog.GroupName(), insertId);
        }
    }
}

void MainWindow::OnSetTorchlight2FolderClicked()
{
    QString folder = QFileDialog::getOpenFileName(this, "Choose folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        ui->SettingsTabTorchlight2FolderLineEdit->setText(folder);
        ui->SettingsTabTorchlight2FolderLineEdit->setToolTip(folder);
        mOptions.Set(OptionKeys::Torchlight2SharedStashFile, folder);
    }
}

void MainWindow::OnSetStashesFolderClicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Choose folder");

    if (!folder.isNull() && !folder.isEmpty())
    {
        ui->SettingsTabStashesFolderLineEdit->setText(folder);
        ui->SettingsTabStashesFolderLineEdit->setToolTip(folder);
        mOptions.Set(OptionKeys::StashManagerFolder, folder);
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
    QString torchlight2Folder = mOptions[OptionKeys::Torchlight2SharedStashFile];
    QString stashesFolder = mOptions[OptionKeys::StashManagerFolder];

    QString decryptedFilePath;
    QString reencryptedFilePath;

    if (stashesFolder.length() > 0)
    {
        decryptedFilePath = stashesFolder + "/decryptedStash.bin";

        reencryptedFilePath = stashesFolder + "/re-encryptedStash.bin";
    }

    Torchlight2StashConverter::DescrambleFile(torchlight2Folder, decryptedFilePath);
    Torchlight2StashConverter::ScrambleFile(decryptedFilePath, reencryptedFilePath);


}
