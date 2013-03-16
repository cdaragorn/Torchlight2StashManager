#include "infinitestashtreeview.h"

#include <QDragMoveEvent>
#include <QStandardItemModel>
#include <QMimeData>
#include <QDebug>

InfiniteStashTreeView::InfiniteStashTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void InfiniteStashTreeView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    QTreeView::rowsInserted(parent, start, end);
//    QStandardItemModel* model = (QStandardItemModel*)this->model();

//    QStandardItem* test = model->itemFromIndex(parent);
//    QStandardItem* child = test->child(start);
//    QStandardItem* item = model->item(start);

//    if (item != NULL)
//        item->setDropEnabled(false);
}

void InfiniteStashTreeView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
}

void InfiniteStashTreeView::dropEvent(QDropEvent *event)
{
    QTreeView::dropEvent(event);
    itemDropped(event);

}

bool InfiniteStashTreeView::wasDropIndicatorOnItem()
{
    return dropIndicatorPosition() == QTreeView::OnItem;
}

qint8 InfiniteStashTreeView::whereWasDropIndicator()
{
    qint8 dropPos = dropIndicatorPosition();
//    qint8 dropPos = dropIndicatorPosition();
//    QTreeView::DropIndicatorPosition pos = (QTreeView::DropIndicatorPosition)dropIndicatorPosition();
    qint8 result = 0;

    switch (dropPos)
    {
        case QTreeView::AboveItem:
        {
            result = -1;
            break;
        }

        case QTreeView::BelowItem:
        {
            result = 0;
            break;
        }

        case QTreeView::OnItem:
        {
            result = 1;
            break;
        }
    }

    return result;
}

void InfiniteStashTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeView::dragMoveEvent(event);
    QStandardItem* itemDraggedOver = ((QStandardItemModel*)model())->itemFromIndex(indexAt(event->pos()));

    if (itemDraggedOver == NULL)
    {
        //dragging over viewport, reject
        event->ignore();
        event->setDropAction(Qt::IgnoreAction);
    }
    else if(itemDraggedOver->parent() == NULL)
    {
        if (!wasDropIndicatorOnItem())
        {
            //dragging over element without parent and drop would drop
            //outside of this element, reject
            event->ignore();
            event->setDropAction(Qt::IgnoreAction);
        }
    }

}
