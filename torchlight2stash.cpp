#include "torchlight2stash.h"

#include <QDebug>
#include <QDataStream>
#include <tools.h>

Torchlight2Stash::Torchlight2Stash(QByteArray& sharedStash, QObject *parent) :
    QObject(parent)
{
    mStash = sharedStash;
    ReadItemsInStash();
}

void Torchlight2Stash::ReadItemsInStash()
{
    qint32 numItemsInStash = 0;

    numItemsInStash = convert<qint32>(&mStash.data()[5]);

    qDebug() << "Number of items in your stash: " << numItemsInStash;

    qint64 itemRecordStartPos = 9;

    for (qint32 i = 0; i < numItemsInStash; ++i)
    {
        qint32 itemRecordSize = convert<qint32>(&mStash.data()[itemRecordStartPos]);

        QByteArray itemArray = mStash.mid(itemRecordStartPos, itemRecordSize + 4);

        Torchlight2Item nextItem(itemArray);

        mItemsInStash.append(nextItem);

        itemRecordStartPos += itemRecordSize + 4;
    }
}
