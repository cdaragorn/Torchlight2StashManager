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

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mGroupsTable("database.sqlite")
{
    ui->setupUi(this);

    ui->MainTabPage->SetSettingsTabPage(ui->SettingsTabPage);

    InfiniteStashStandardItemModel* model = new InfiniteStashStandardItemModel();
    LoadGroups(model);

    ui->MainTabPage->SetInfiniteStashStandardItemModel(model);


    ui->ManageGroupsTab->SetGroupsTable(&mGroupsTable);
    ui->ManageGroupsTab->SetGroupManagerStandardItemModel(model);

    connect(ui->actionCreateNewStash, SIGNAL(triggered()), this, SLOT(OnCreateNewStashClicked()));

    ui->MainToolBar->addAction(QWhatsThis::createAction());

    connect(&mOptions, SIGNAL(OptionsChanged(QString, QString)), this, SLOT(OnOptionsChanged()));

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

        ui->SettingsTabPage->Options(&mOptions);
        ui->MainTabPage->Options(&mOptions);
//        ui->MainTab->Options(&mOptions);
    }

}

void MainWindow::LoadGroups(InfiniteStashStandardItemModel* inModel)
{
    QList<Group> rootItems = mGroupsTable.GetAllTopLevelGroups();

    for (int i = 0; i < rootItems.length(); ++i)
    {
        Group nextGroup = rootItems[i];

        if (nextGroup.groupId > 0)
        {
            QStandardItem* nextItem = new QStandardItem(nextGroup.groupName);
            nextItem->setData(nextGroup.groupId, Qt::UserRole);
            nextItem->setData(InfiniteStashStandardItemModel::Group, Qt::UserRole + 1);
            QIcon icon("://images/Open-Folder.png");
            nextItem->setIcon(icon);
            inModel->appendRow(nextItem);
        }
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

