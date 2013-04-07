#include "torchlight2item.h"

#include <tools.h>
#include <QVector>

Torchlight2Item::Torchlight2Item(QByteArray itemBytes)
{
    mItemBytes = itemBytes;
    mItemNameEndPos = 0;
    mNumSockets = 0;
    mNumFilledSockets = 0;
    ParseItemDetails();
}


Torchlight2Item::~Torchlight2Item()
{
}

qint16 Torchlight2Item::Index()
{
    qint32 itemIndexPos = m24ffBlockEndPos + 8;
    return convert<qint16>(&mItemBytes.data()[itemIndexPos]);
}

void Torchlight2Item::SetIndex(quint16 index)
{
    qint32 itemIndexPos = m24ffBlockEndPos + 8;

    reinterpret_cast<quint16*>(&mItemBytes.data()[itemIndexPos])[0] = index;
}

bool Torchlight2Item::operator ==(const Torchlight2Item& other) const
{
    return mItemBytes == other.mItemBytes;
}

/*
 *  Parses out all item data we know how to from
 *  the item's byte array
 */
void Torchlight2Item::ParseItemDetails()
{
    mItemUniqueTypeId = convert<qint64>(&mItemBytes.data()[5]);

    qint32 itemNameLength = convert<qint16>(&mItemBytes.data()[13]);
    mItemNameEndPos = 15 + (itemNameLength * 2);

    qint64 nameStartPos = 15;
    QVector<quint16> rawItemName;

    bool appendNextCharacter = true;
    qint64 i = nameStartPos;

    while (itemNameLength > 0)
    {
        mItemNameEndPos = i + (itemNameLength * 2);
        for (; i < mItemNameEndPos; i += 2)
        {
    //            quint16 character = convert<quint16>(&mStash.data()[i]);
    //            QChar character = convert<QChar>(&itemArray->data()[i]);
            quint16 wcharacter = convert<quint16>(&mItemBytes.data()[i]);
            QChar something(wcharacter);

            if (something == '[')
            {
                appendNextCharacter = false;
            }


            if (appendNextCharacter)
            {
                rawItemName.append(wcharacter);
            }
            else if (something == ']')
            {
                appendNextCharacter = true;
            }
    //            QString unicodeResult = QString::fromUtf16(&wcharacter, 1);
    //            itemName += unicodeResult;
        }
        itemNameLength = convert<qint16>(&mItemBytes.data()[i]);
        i += 2;
    }

    mItemName = QString::fromUtf16(&rawItemName[0], rawItemName.size());

    //find the block of 24 ff's that should occur shortly after the end of the item name
    QByteArray ffBlock(24, 0xff);
    m24ffBlockEndPos = mItemBytes.indexOf(ffBlock, mItemNameEndPos) + 24;

    mNumSockets = convert<qint32>(&mItemBytes.data()[m24ffBlockEndPos + 115]);
    mNumFilledSockets = convert<qint32>(&mItemBytes.data()[m24ffBlockEndPos + 119]);

}
