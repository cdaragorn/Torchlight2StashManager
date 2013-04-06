#include "groupmanagertreeview.h"
#include <QStandardItemModel>
#include <infinitestashstandarditemmodel.h>
#include <QContextMenuEvent>

GroupManagerTreeView::GroupManagerTreeView(QWidget *parent) :
    QTreeView(parent)
{
    mContextMenu = new QMenu(this);
    QAction* action = new QAction("Remove Group", mContextMenu);
    connect(action, SIGNAL(triggered()), this, SLOT(OnDeleteGroupClicked()));
    mContextMenu->addAction(action);
}

GroupManagerTreeView::~GroupManagerTreeView()
{
    if (mContextMenu != NULL)
        delete mContextMenu;
}

void GroupManagerTreeView::contextMenuEvent(QContextMenuEvent *inEvent)
{
    QTreeView::contextMenuEvent(inEvent);

    if (mContextMenu != NULL)
        mContextMenu->popup(inEvent->globalPos());
}

void GroupManagerTreeView::OnDeleteGroupClicked()
{
    QPoint point = this->mapFromGlobal(mContextMenu->pos());
    QStandardItemModel* model = dynamic_cast<QStandardItemModel*>(this->model());
    QStandardItem* itemClicked = model->itemFromIndex(this->indexAt(point));

    if (itemClicked != NULL)
    {
        QVariant itemType = itemClicked->data(Qt::UserRole + 1);

        if (!itemType.isNull() && itemType.toInt() == InfiniteStashStandardItemModel::GroupItemType)
        {
            if (!itemClicked->hasChildren())
            {
                QVariant groupId = itemClicked->data(Qt::UserRole);

                if (!groupId.isNull())
                {
                    QStandardItem* parent = itemClicked->parent();

                    if (parent == NULL)
                    {
                        model->removeRow(itemClicked->row());
                    }
                    else
                    {
                        parent->removeRow(itemClicked->row());
                    }

                    groupDeleted(groupId.toLongLong());
                }
            }
        }
    }
}
