#include "customlistwidget.h"

CustomListWidget::CustomListWidget(QWidget *parent) :
    QListWidget(parent)
{
}


void CustomListWidget::rowsInserted(const QModelIndex &parent, int start, int end)
{
    QListWidget::rowsInserted(parent, start, end);

    for (qint32 i = start; i <= end; ++i)
    {
        itemAdded(item(i));
    }
}

void CustomListWidget::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    QListWidget::rowsAboutToBeRemoved(parent, start, end);

    QList<QListWidgetItem*> items;
    for (qint32 i = start; i <= end; ++i)
    {
        items.append(item(i));
    }

    itemsAboutToBeRemoved(items);
}
