#include "infinitestashstandarditemmodel.h"

#include <QListWidgetItem>
#include <QDebug>

InfiniteStashStandardItemModel::InfiniteStashStandardItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
}


Qt::ItemFlags InfiniteStashStandardItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QStandardItemModel::flags(index);

    if (index.isValid())
    {
        QStandardItem* raw = itemFromIndex(index);

        result = raw->flags();

        if ((result & Qt::ItemIsDragEnabled))
        {
            result &= ~Qt::ItemIsDropEnabled;
        }

    }

    return result;
}
