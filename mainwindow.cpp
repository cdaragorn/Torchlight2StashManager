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
#include <torchlight2item.h>
#include <QStandardItemModel>
#include <QStandardItem>
#include <infinitestashstandarditemmodel.h>

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
    ui->MainTab->SetGroupsTable(&mGroupsTable);
    ui->MainTab->SetNumberOfItemsInSharedStashLabel(ui->MainTabNumberOfItemsInSharedStashLabel);
    ui->MainTab->SetGroupsComboBox(ui->MainTabGroupsComboBox);
    ui->MainTab->SetSettingsTabPage(ui->SettingsTab);


    InfiniteStashStandardItemModel* model = new InfiniteStashStandardItemModel();
    QStandardItem* parentItem = new QStandardItem("root");
    parentItem->setDragEnabled(false);
    parentItem->setEditable(false);
    QIcon icon("://images/Open-Folder.png");
    parentItem->setIcon(icon);
    model->appendRow(parentItem);

    for (int i = 0; i < 4; ++i)
    {
        QStandardItem* item = new QStandardItem(QString("item %0").arg(i));
        item->setEditable(false);
        item->setDragEnabled(false);
        item->setIcon(icon);
        parentItem->appendRow(item);
        parentItem = item;
    }

    QStringList labels;
    labels.append("one");
    labels.append("two");
    labels.append("three");
    labels.append("four");

//    model->setVerticalHeaderLabels(labels);

    ui->TestTreeView->setModel(model);

    ui->MainTab->SetInfiniteStashTreeView(ui->TestTreeView);
    ui->MainTab->SetInfiniteStashTreeViewModel(model);


    ui->ManageGroupsTab->SetGroupsTable(&mGroupsTable);
    ui->ManageGroupsTab->SetGroupNameLineEdit(ui->ManageGroupsTabGroupNameLineEdit);
    ui->ManageGroupsTab->SetAddGroupButton(ui->ManagerGroupsTabAddGroupButton);
    ui->ManageGroupsTab->SetGroupManagerTreeView(ui->ManageGroupsTabGroupManagerTreeView);


//    ui->MainTabControlWidget->move(0, 0);
//    QLayout* layout = ui->MainTab->layout();
//    QPushButton* button = new QPushButton(ui->MainTab);
//    button->setText("Boogey");
//    button->move(180, 50);
//    button->resize(80, 30);
//    connect(button, SIGNAL(pressed()), this, SLOT(OnBoogeyClicked()));

    connect(ui->actionCreateNewStash, SIGNAL(triggered()), this, SLOT(OnCreateNewStashClicked()));

    ui->MainToolBar->addAction(QWhatsThis::createAction());

//    connect(ui->AddGroupButton, SIGNAL(pressed()), this, SLOT(OnAddGroup()));

    connect(&mOptions, SIGNAL(OptionsChanged(QString, QString)), this, SLOT(OnOptionsChanged()));

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

