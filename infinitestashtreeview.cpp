#include "infinitestashtreeview.h"

#include <QDragMoveEvent>
#include <QStandardItemModel>
#include <QMimeData>
#include <QDebug>
#include <infinitestashstandarditemmodel.h>

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

/*
 *  If the drag was started from the infinite stash tree view,
 *  we need to check to see if the user is trying to drag a group.
 *  If that is the case, we need to reject the drag event, as groups
 *  should not be dragged on this control.
 */
void InfiniteStashTreeView::dragEnterEvent(QDragEnterEvent *event)
{
    QTreeView::dragEnterEvent(event);

    if (event->source() == this)
    {
        QByteArray modelList = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        QDataStream inputStream(&modelList, QIODevice::ReadOnly);

        while (!inputStream.atEnd())
        {
            int row;
            int col;
            QMap<int, QVariant> roleDataMap;
            inputStream >> row >> col >> roleDataMap;
            QVariant itemType = roleDataMap[Qt::UserRole + 1];

            if (itemType.toInt() == InfiniteStashStandardItemModel::Group)
            {
                event->ignore();
                event->setDropAction(Qt::IgnoreAction);
            }
        }
    }
}

/*
 *  We have to check all dragging to make sure we don't drop items
 *  somewhere we can't allow them to be.  Items must meet the following
 *  rules:
 *
 *  1. All items must be in a group.
 *  2. A stash item cannot have anything set as its child.
 *
 *
 */
void InfiniteStashTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeView::dragMoveEvent(event);
    QStandardItem* itemDraggedOver = ((QStandardItemModel*)model())->itemFromIndex(indexAt(event->pos()));

    bool acceptEvent = true;

    if (itemDraggedOver == NULL)
    {
        //dragging over viewport, reject
        acceptEvent = false;
    }
    else
    {
        QVariant itemType = itemDraggedOver->data(Qt::UserRole + 1);

        if (itemType.toInt() == InfiniteStashStandardItemModel::StashItem)
        {
            //dragging over another stash item and drop would
            //drop on the stash item, reject
            if (wasDropIndicatorOnItem())
                acceptEvent = false;
        }
        else if(itemDraggedOver->parent() == NULL)
        {
            if (!wasDropIndicatorOnItem())
            {
                //dragging over element without parent and drop would drop
                //outside of this element, reject
                acceptEvent = false;
            }
        }
    }

    if (!acceptEvent)
    {
        event->ignore();
        event->setDropAction(Qt::IgnoreAction);
    }

}
