#include "torchlight2stash.h"

#include <QDebug>
#include <tools.h>

const qint32 Torchlight2Stash::GeneralTabIndexStart = 3322;
const qint32 Torchlight2Stash::PotionsTabIndexStart = 4322;
const qint32 Torchlight2Stash::SpellsTabIndexStart = 5322;

Torchlight2Stash::Torchlight2Stash(QByteArray& sharedStash)
{
    mStash = sharedStash;

    //Torchlight 2s default is 40
    mMaximumNumberOfItemsPerTab = 40;

    ClearNumberOfItemsInTabs();

    ReadItemsInStash();
}

void Torchlight2Stash::ReadItemsInStash()
{
    qint32 numItemsInStash = 0;

    numItemsInStash = convert<qint32>(&mStash.data()[5]);

    qDebug() << "Number of items in your stash: " << numItemsInStash;
    mItemsInStash.clear();

    //This is just a sanity check to make sure we don't blow up the systems memory
    //because of a bad file
    if (numItemsInStash <= 1000000)
    {
        mItemsInStash.reserve(numItemsInStash);

        qint64 itemRecordStartPos = 9;

        for (qint32 i = 0; i < numItemsInStash; ++i)
        {
            qint32 itemRecordSize = convert<qint32>(&mStash.data()[itemRecordStartPos]);

            QByteArray itemArray = mStash.mid(itemRecordStartPos, itemRecordSize + 4);

            Torchlight2Item nextItem(itemArray);

            //reset each items index so we don't have to worry about holes in the stash
            //this ends up packing the persons stash on them, but it helps avoid
            //planting an item in the same spot as an existing one
            nextItem.SetIndex(CalculateItemIndex(nextItem));
            mItemsInStash.append(nextItem);

            itemRecordStartPos += itemRecordSize + 4;
        }
    }
}

void Torchlight2Stash::ClearNumberOfItemsInTabs()
{
    mNumberOfItemsInTab[GeneralStashTab] = 0;
    mNumberOfItemsInTab[PotionsStashTab] = 0;
    mNumberOfItemsInTab[SpellsStashTab] = 0;
}

quint16 Torchlight2Stash::CalculateItemIndex(const Torchlight2Item &item)
{
    qint32 tabIndexPosition = item.GetItemNameEndPosition();
    qint16 tabIndex = convert<qint16>(&item.Bytes().data()[tabIndexPosition + 67]);
    quint16 itemStashIndex = GeneralTabIndexStart;

    switch (tabIndex)
    {
        case GeneralStashTab:
        {
            itemStashIndex = GeneralTabIndexStart + mNumberOfItemsInTab[GeneralStashTab]++;
            break;
        }

        case PotionsStashTab:
        {
            itemStashIndex = PotionsTabIndexStart + mNumberOfItemsInTab[PotionsStashTab]++;
            break;
        }

        case SpellsStashTab:
        {
            itemStashIndex = SpellsTabIndexStart + mNumberOfItemsInTab[SpellsStashTab]++;
            break;
        }
    }
    return itemStashIndex;
}

void Torchlight2Stash::AddItemToStash(Torchlight2Item item)
{
    item.SetIndex(CalculateItemIndex(item));

    mItemsInStash.append(item);
    mStash.append(item.Bytes());
    reinterpret_cast<qint32*>(&mStash.data()[5])[0] = mItemsInStash.length();
}

void Torchlight2Stash::RemoveItemFromStash(Torchlight2Item item)
{
    if (mItemsInStash.removeOne(item))
    {
        reinterpret_cast<qint32*>(&mStash.data()[5])[0] = mItemsInStash.length();
        ClearNumberOfItemsInTabs();
        mStash.truncate(9);

        for (int i = 0; i < mItemsInStash.length(); ++i)
        {
            mItemsInStash[i].SetIndex(CalculateItemIndex(mItemsInStash[i]));
            mStash.append(mItemsInStash[i].Bytes());
        }
    }
}

bool Torchlight2Stash::CanItemBeAdded(Torchlight2Item item)
{
    bool result = false;

    qint32 tabIndexPosition = item.GetItemNameEndPosition();
    qint16 tabIndex = convert<qint16>(&item.Bytes().data()[tabIndexPosition + 63]);

    switch (tabIndex)
    {
        case GeneralStashTab:
        {
            result = mNumberOfItemsInTab[GeneralStashTab] < mMaximumNumberOfItemsPerTab;
            break;
        }

        case PotionsStashTab:
        {
            result = mNumberOfItemsInTab[PotionsStashTab] < mMaximumNumberOfItemsPerTab;
            break;
        }

        case SpellsStashTab:
        {
            result = mNumberOfItemsInTab[SpellsStashTab] < mMaximumNumberOfItemsPerTab;
            break;
        }
    }

    return result;
}
