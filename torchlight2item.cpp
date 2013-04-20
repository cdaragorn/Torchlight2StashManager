#include "torchlight2item.h"

#include <tools.h>
#include <QVector>
#include <QDebug>

const QString Torchlight2Item::ItemDetailsDatabase = "itemDatabase.sqlite";

Torchlight2Item::Torchlight2Item(QByteArray itemBytes)
{
    mItemBytes = itemBytes;
    mItemNameEndPos = 0;
    mNumSockets = 0;
    mNumFilledSockets = 0;
    mPhysicalArmor = 0;
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

    mItemName = ParseName(13, mItemNameEndPos);

    //find the block of 24 ff's that should occur shortly after the end of the item name
    QByteArray ffBlock(24, 0xff);
    m24ffBlockEndPos = mItemBytes.indexOf(ffBlock, mItemNameEndPos) + 24;
}


void Torchlight2Item::ParseAdvancedItemDetails(QDir infiniteStashFolder)
{
    EffectsTable* effectsTable = new EffectsTable(infiniteStashFolder.absoluteFilePath(ItemDetailsDatabase));

    mNumSockets = convert<qint32>(&mItemBytes.data()[m24ffBlockEndPos + 115]);
    mNumFilledSockets = convert<qint32>(&mItemBytes.data()[m24ffBlockEndPos + 119]);

    qint32 socketedItemPos = m24ffBlockEndPos + 123;

    for (qint32 i = 0; i < mNumFilledSockets; ++i)
    {
        SocketedItem nextItem;
        nextItem = ParseSocketedItem(socketedItemPos, effectsTable, socketedItemPos);
        mSocketedItems.append(nextItem);
    }

    QByteArray ff4Block(4, 0xff);
    qint32 physicalArmorPos = mItemBytes.indexOf(ff4Block, socketedItemPos) + 4;
    mPhysicalArmor = convert<qint32>(&mItemBytes.constData()[physicalArmorPos]);

    qint32 numDamageTypesPos = physicalArmorPos + 20;
    if (convert<qint32>(&mItemBytes.constData()[physicalArmorPos + 4]) == -1)
    {
        numDamageTypesPos -= 4;
    }
    qint32 damageTypesCurrentPos = numDamageTypesPos;
    qint8 numDamageTypes = convert<qint8>(&mItemBytes.constData()[numDamageTypesPos]);

    if (numDamageTypes > 0)
    {
        damageTypesCurrentPos += 10;

        //read damage type

        for (qint32 i = 1; i < numDamageTypes; ++i)
        {
            damageTypesCurrentPos += 12;
            //read next one
        }
        damageTypesCurrentPos += 2;
    }

    qint32 effectCountPos = damageTypesCurrentPos + 2;
    qint32 numMagicalEffects = convert<qint32>(&mItemBytes.constData()[effectCountPos]);

    while (numMagicalEffects > 0)
    {
        qint32 effectPos = effectCountPos + 4;

        for (qint32 i = 0; i < numMagicalEffects; ++i)
        {
            QString effectDescription;

            switch (ParseEffect(effectDescription, effectPos, effectsTable, effectPos))
            {
                case Torchlight2Item::MagicArmorEffectType:
                {
                    mMagicalArmors.append(effectDescription);
                    break;
                }

                case Torchlight2Item::EnchantmentEffectType:
                {
                    mEnchantments.append(effectDescription);
                    break;
                }

                case Torchlight2Item::GeneralEffectType:
                default:
                {
                    mMagicalEffects.append(effectDescription);
                    break;
                }
            }

        }
        effectCountPos = effectPos;
        numMagicalEffects = convert<qint32>(&mItemBytes.constData()[effectCountPos]);
    }
    delete effectsTable;
}

QString Torchlight2Item::ParseName(qint32 nameStartPos, qint32 &nameEndPos)
{
    qint32 itemNameLength = convert<qint16>(&mItemBytes.data()[nameStartPos]);
    nameEndPos = nameStartPos + 2 + (itemNameLength * 2);

    qint64 i = nameStartPos + 2;
    QVector<quint16> rawItemName;
    bool appendNextCharacter = true;

    while (itemNameLength > 0)
    {
        nameEndPos = i + (itemNameLength * 2);
        for (; i < nameEndPos; i += 2)
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

    QString result = QString::fromUtf16(&rawItemName[0], rawItemName.size());

    return result;

}

SocketedItem Torchlight2Item::ParseSocketedItem(qint32 itemStartPos, EffectsTable* effectsTable, qint32 &itemEndPos)
{
    SocketedItem itemDetails;

    qint32 itemNameEndPos = 0;
    itemDetails.name = ParseName(itemStartPos + 9, itemNameEndPos);

    QByteArray ff24Block(24, 0xff);

    qint32 item24BlockEndPos = mItemBytes.indexOf(ff24Block, itemNameEndPos) + 24;

    QByteArray ff4Block(4, 0xff);

    qint32 item4BlockEndPos = mItemBytes.indexOf(ff4Block, item24BlockEndPos + 123) + 4;

    qint32 itemNumberOfEffectsPos = item4BlockEndPos + 22;

    qint32 numberOfEffects = convert<qint32>(&mItemBytes.constData()[itemNumberOfEffectsPos]);
    itemEndPos = itemNumberOfEffectsPos + 4;

    ParseEffect(itemDetails.effect, itemEndPos, effectsTable, itemEndPos);

    //parse through any other effects that are on this socketed item
    //to get their end points, but we don't care what they are
    for (qint32 i = 1; i < numberOfEffects; ++i)
    {
        QString description;
        ParseEffect(description, itemEndPos, effectsTable, itemEndPos);
    }

    itemEndPos += 16;

    return itemDetails;
}

Torchlight2Item::EffectType Torchlight2Item::ParseEffect(QString &effectDescription, qint32 effectStartPos,
                                                         EffectsTable* effectsTable, qint32 &effectEndPos)
{
    Torchlight2Item::EffectType effectTypeResult = Torchlight2Item::GeneralEffectType;

    quint32 effectFlags = convert<quint32>(&mItemBytes.constData()[effectStartPos]);

    if ((effectFlags & 0x0400) != 0)
    {
        effectTypeResult = Torchlight2Item::EnchantmentEffectType;
    }
    else if ((effectFlags & 0x0002) != 0)
    {
        effectTypeResult = Torchlight2Item::MagicArmorEffectType;
    }

    qint16 effectNameLength = convert<qint16>(&mItemBytes.constData()[effectStartPos + 4]);

//    qint32 effectNameEndPos = effectStartPos + 4;
//    QString effectName = ParseName(effectNameEndPos, effectNameEndPos);
    qint32 currentPos = effectStartPos + 6 + (effectNameLength * 2);

    if ((effectFlags & 0x0800) != 0)
    {
        //bad
        qDebug() << "huh?";
    }
    else if ((effectFlags & 0x1000) != 0)
    {
        //bad
        qDebug() << "huh?";
    }
    else if ((effectFlags & 0x4000) != 0)
    {
        //bad
        qDebug() << "huh?";
    }

        if ((effectFlags & 0x0100) != 0)
        {
            qint32 nextStringLength = convert<qint16>(&mItemBytes.constData()[currentPos]);
            currentPos += 2 + (nextStringLength * 2);
        }

        if ((effectFlags & 0x2000) != 0)
        {
            //some unknown 64 bit number here
            currentPos += 8;
        }

        qint8 numAdditionalValues = mItemBytes[currentPos];
        ++currentPos;
        float additionalValues[numAdditionalValues];

        for (qint32 i = 0; i < numAdditionalValues; ++i)
        {
            additionalValues[i] = convert<float>(&mItemBytes.constData()[currentPos]);
            currentPos += 4;
        }

        if (convert<qint16>(&mItemBytes.constData()[currentPos]) != 0)
        {
            qDebug() << "Error: effect count > 0, unsupported";
        }

        currentPos += 2;

        qint32 effectNumber = convert<qint32>(&mItemBytes.constData()[currentPos]);
        currentPos += 4;
        Torchlight2Item::DamageType damageType = (Torchlight2Item::DamageType)convert<qint32>(&mItemBytes.constData()[currentPos]);
        currentPos += 12;

        float effectDuration = convert<float>(&mItemBytes.constData()[currentPos]);
        currentPos += 8;
        float effectStrength = convert<float>(&mItemBytes.constData()[currentPos]);

        Effect effectDetails = effectsTable->GetEffect(effectNumber);

        if (effectDuration == -1000)
        {
            effectDescription = (effectStrength >= 0) ?
                        effectDetails.goodDescription : effectDetails.badDescription;

        }
        else
        {
            effectDescription = (effectStrength >= 0) ?
                        effectDetails.goodDescriptionOverTime : effectDetails.badDescriptionOverTime;

            effectDescription.replace("[DURATION]", QString::number(effectDuration, 'f', 0));
        }


        effectDescription.replace("[VALUE]", QString::number(effectStrength, 'f', 0));
        effectDescription.replace("[DMGTYPE]", GetDamageTypeDescription(damageType));

        for (qint32 i = 0; i < numAdditionalValues; ++i)
        {
            effectDescription.replace("[VALUE" + QString::number(i + 1) + "]", QString::number(additionalValues[i], 'f', 0));
        }

        effectEndPos = currentPos + 8;


    return effectTypeResult;
}

QString Torchlight2Item::GetDamageTypeDescription(DamageType damageType)
{
    QString result = "";
    switch (damageType)
    {
        case Torchlight2Item::PhysicalDamageType:
        {
            result = "Physical";
            break;
        }

        case Torchlight2Item::MagicalDamageType:
        {
            result = "Magical";
            break;
        }

        case Torchlight2Item::FireDamageType:
        {
            result = "Fire";
            break;
        }

        case Torchlight2Item::IceDamageType:
        {
            result = "Ice";
            break;
        }

        case Torchlight2Item::ElectricDamageType:
        {
            result = "Electric";
            break;
        }

        case Torchlight2Item::PoisonDamageType:
        {
            result = "Poison";
            break;
        }

        case Torchlight2Item::AllDamageTypes:
        {
            result = "All";
            break;
        }
    }

    return result;
}
