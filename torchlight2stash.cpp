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
//    QDataStream stream(mStash);
//    stream.setByteOrder(QDataStream::LittleEndian);

//    stream.skipRawData(5);

    qint32 numItemsInStash = 0;

//    stream >> numItemsInStash;

    numItemsInStash = convert<qint32>(&mStash.data()[5]);

//    qint32 numItemsInStash = reinterpret_cast<const qint32*>(&mStash.constData()[5])[0];

    qDebug() << "Number of items in your stash: " << numItemsInStash;

//    stream.skipRawData(13);

    qint64 itemRecordStartPos = 9;

    for (qint32 i = 0; i < numItemsInStash; ++i)
    {
        qint32 itemRecordSize = convert<qint32>(&mStash.data()[itemRecordStartPos]);

        QByteArray* itemArray = new QByteArray(mStash.mid(itemRecordStartPos, itemRecordSize + 4));

        qint64 itemUniqueTypeId = convert<qint64>(&itemArray->data()[5]);

        qint32 itemNameLength = convert<qint16>(&itemArray->data()[13]);

        qint64 nameStartPos = 15;
        QString itemName;

        for (qint64 i = 15; i < (nameStartPos + (itemNameLength * 2)); i += 2)
        {
//            quint16 character = convert<quint16>(&mStash.data()[i]);
            QChar character = convert<QChar>(&itemArray->data()[i]);
            itemName += character;
        }

        if (itemName.length() > 0 && !mItemsInStash.contains(itemName))
            mItemsInStash[itemName] = itemArray;


//        qDebug() << "Item Record size: " << itemRecordSize << "\nItem Type ID: " << itemUniqueTypeId <<
//                    "\nItem Name: " << itemName;

        itemRecordStartPos += itemRecordSize + 4;
//        stream >> itemRecordSize;
    }
}
