#include "sharedstashlistwidget.h"

#include <iostream>
using namespace std;

SharedStashListWidget::SharedStashListWidget(QWidget *parent) :
    QListWidget(parent)
{
    mContextMenu = new QMenu(this);
    QAction* action = new QAction("hello", mContextMenu);
    connect(action, SIGNAL(triggered()), this, SLOT(OnAction()));
    mContextMenu->addAction(action);
//    setContextMenuPolicy(Qt::CustomContextMenu);
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
//    if (event != NULL)
//    {
//        event->acceptProposedAction();
//    }
}
