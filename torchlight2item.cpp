#include "torchlight2item.h"

#include <tools.h>
#include <QVector>

Torchlight2Item::Torchlight2Item(QByteArray itemBytes)
{
    mItemBytes = itemBytes;
    mItemNameEndPos = 0;
    ParseItemDetails();
}


Torchlight2Item::~Torchlight2Item()
{
}

void Torchlight2Item::SetIndex(quint16 index)
{
    qint32 itemIndexPos = mItemNameEndPos + 61;

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

    for (qint64 i = nameStartPos; i < (nameStartPos + (itemNameLength * 2)); i += 2)
    {
//            quint16 character = convert<quint16>(&mStash.data()[i]);
//            QChar character = convert<QChar>(&itemArray->data()[i]);
        quint16 wcharacter = convert<quint16>(&mItemBytes.data()[i]);
        rawItemName.append(wcharacter);
//            QString unicodeResult = QString::fromUtf16(&wcharacter, 1);
//            itemName += unicodeResult;
    }

    mItemName = QString::fromUtf16(&rawItemName[0], rawItemName.size());
}
