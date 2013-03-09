#include "torchlight2stash.h"

#include <QDebug>
#include <QDataStream>

Torchlight2Stash::Torchlight2Stash(QByteArray& sharedStash, QObject *parent) :
    QObject(parent)
{
    mStash = sharedStash;
    ReadItemsInStash();
}

void Torchlight2Stash::ReadItemsInStash()
{
    QDataStream stream(mStash);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream.skipRawData(5);

    qint32 numItemsInStash = 0;

    stream >> numItemsInStash;

//    qint32 numItemsInStash = reinterpret_cast<const qint32*>(&mStash.constData()[5])[0];

    qDebug() << "Number of items in your stash: " << numItemsInStash;

    stream.skipRawData(13);

    for (qint32 i = 0; i < numItemsInStash; ++i)
    {

    }
}
