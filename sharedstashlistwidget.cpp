#include "sharedstashlistwidget.h"

#include <iostream>
using namespace std;

#include <QMimeData>

SharedStashListWidget::SharedStashListWidget(QWidget *parent) :
    CustomListWidget(parent)
{
    mContextMenu = new QMenu(this);
    QAction* action = new QAction("hello", mContextMenu);
    connect(action, SIGNAL(triggered()), this, SLOT(OnAction()));
    mContextMenu->addAction(action);
//    setContextMenuPolicy(Qt::CustomContextMenu);
}

SharedStashListWidget::~SharedStashListWidget()
{
    if (mContextMenu != NULL)
        delete mContextMenu;
}

void SharedStashListWidget::contextMenuEvent(QContextMenuEvent * inEvent)
{
    mContextMenu->popup(inEvent->globalPos());
}

void SharedStashListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if (event->button() == Qt::RightButton)
    {

    }
}

QListWidgetItem* SharedStashListWidget::GetDroppedItem(QPoint inPosition)
{
    QListWidgetItem* result = NULL;

    QModelIndex itemIndex = indexAt(inPosition);
    result = itemFromIndex(itemIndex);

    if (dropIndicatorPosition() == QListWidget::BelowItem)
    {
        result = item(row(result) + 1);
    }

    return result;
}

void SharedStashListWidget::OnAction()
{
    stringstream text;
    QPoint point = this->mapFromGlobal(mContextMenu->pos());
    QListWidgetItem* itemClicked = this->itemAt(point);
    text << "Context Menu Pos: " << point.x() << ", " << point.y();

    if (itemClicked != NULL)
    {
        text << endl << "Clicked on: " << itemClicked->text().toStdString();
    }
    QMessageBox::information(NULL, "Action called", text.str().c_str(),
                             QMessageBox::Ok);
}

void SharedStashListWidget::itemChanged(QListWidgetItem *item)
{
    QListWidget::itemChanged(item);
    if (item != NULL)
    {
        QMessageBox::information(this, "something", item->text(), QMessageBox::Ok);
    }
}

void SharedStashListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    itemDropped(event);

//    if (dropIndicatorPosition() == QListWidget::BelowItem)
//    {
//        QListWidgetItem* itemOne = itemFromIndex(indexAt(event->pos()));
//        QListWidgetItem* test = itemAt(event->pos());

//        QListWidgetItem* testTwo = item(row(test) + 1);

//        int f = 4;
//    }

//    if (item != NULL)
//    {
//        int f = 3;
//    }
//    if (event != NULL)
//    {
//        event->acceptProposedAction();
//    }
}

void SharedStashListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QListWidget::dragEnterEvent(event);

//    QByteArray modelList = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
//    QDataStream inputStream(&modelList, QIODevice::ReadOnly);

//    while (!inputStream.atEnd())
//    {
//        int row;
//        int col;
//        QMap<int, QVariant> roleDataMap;
//        inputStream >> row >> col >> roleDataMap;
//        QVariant itemType = roleDataMap[Qt::UserRole + 1];
//        QVariant itemData = roleDataMap[Qt::UserRole];

////        if (itemType.toInt() == InfiniteStashStandardItemModel::Group)
////        {
////            event->ignore();
////            event->setDropAction(Qt::IgnoreAction);
////        }
//    }
    dragEntered(event);
}

