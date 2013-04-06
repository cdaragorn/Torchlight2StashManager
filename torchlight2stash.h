#ifndef TORCHLIGHT2STASH_H
#define TORCHLIGHT2STASH_H

#include <QObject>
#include <QHash>
#include <torchlight2item.h>

class Torchlight2Stash
{
public:
    explicit Torchlight2Stash(QByteArray& sharedStash);

    enum ItemTabPage
    {
        GeneralStashTab = 24,
        PotionsStashTab = 25,
        SpellsStashTab = 26
    };
    
    QList<Torchlight2Item> StashItems() const { return mItemsInStash; }
    void AddItemToStash(Torchlight2Item item);
    void RemoveItemFromStash(Torchlight2Item item);

    qint32 GetNumberOfItemsInTabPage(ItemTabPage tabPage) const { return mNumberOfItemsInTab[tabPage]; }

    QByteArray Bytes() const { return mStash; }

    void MaximumNumberOfItemsPerTab(qint32 inMaximum)
    {
        mMaximumNumberOfItemsPerTab = inMaximum;
    }


    bool CanItemBeAdded(Torchlight2Item item);


signals:
    
public slots:

private:
    QByteArray mStash;
    qint32 mMaximumNumberOfItemsPerTab;

    QList<Torchlight2Item> mItemsInStash;
    QHash<ItemTabPage, qint32> mNumberOfItemsInTab;


    static const qint32 GeneralTabIndexStart;
    static const qint32 PotionsTabIndexStart;
    static const qint32 SpellsTabIndexStart;

    void ReadItemsInStash();
    quint16 CalculateItemIndex(const Torchlight2Item& item);
    void ClearNumberOfItemsInTabs();
    
};

#endif // TORCHLIGHT2STASH_H
