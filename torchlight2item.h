#ifndef TORCHLIGHT2ITEM_H
#define TORCHLIGHT2ITEM_H

#include <QByteArray>
#include <QString>
#include <QList>
#include <QDir>
#include <effectstable.h>

struct SocketedItem
{
    QString name;
    QString effect;
};

class Torchlight2Item
{

public:
    explicit Torchlight2Item(QByteArray itemBytes);
    ~Torchlight2Item();

    QByteArray Bytes() const { return mItemBytes; }
    QString Name() const { return mItemName; }
    qint32 NumSockets() const { return mNumSockets; }
    qint32 NumFilledSockets() const { return mNumFilledSockets; }

    void ParseAdvancedItemDetails(QDir infiniteStashFolder);

    void SetIndex(quint16 index);
    qint16 Index();
    qint32 GetItemNameEndPosition() const { return mItemNameEndPos; }
    qint64 Get24ffBlockEndPos() const { return m24ffBlockEndPos; }

    bool operator ==(const Torchlight2Item& other) const;

    QList<SocketedItem> SocketedItems() { return mSocketedItems; }
    QList<QString> MagicalEffects() { return mMagicalEffects; }
    QList<QString> Enchantments() { return mEnchantments; }

    qint32 PhysicalArmor() { return mPhysicalArmor; }
    QList<QString> MagicalArmors() { return mMagicalArmors; }
    
//signals:
    
//public slots:

private:

    static const QString ItemDetailsDatabase;

    enum EffectType
    {
        GeneralEffectType,
        MagicArmorEffectType,
        EnchantmentEffectType
    };

    enum DamageType
    {
        PhysicalDamageType = 0,
        MagicalDamageType = 1,
        FireDamageType = 2,
        IceDamageType = 3,
        ElectricDamageType = 4,
        PoisonDamageType = 5,
        AllDamageTypes = 6
    };

    QByteArray mItemBytes;

    qint64 mItemUniqueTypeId;
    QString mItemName;
    qint32 mNumSockets;
    qint32 mNumFilledSockets;

    qint32 mItemNameEndPos;
    qint64 m24ffBlockEndPos;

    qint32 mPhysicalArmor;

    QList<QString> mMagicalArmors;
    QList<SocketedItem> mSocketedItems;
    QList<QString> mMagicalEffects;
    QList<QString> mEnchantments;

    void ParseItemDetails();

    QString ParseName(qint32 nameStartPos, qint32& nameEndPos);

    SocketedItem ParseSocketedItem(qint32 itemStartPos, EffectsTable* effectsTable, qint32& itemEndPos);
    QString GetDamageTypeDescription(DamageType damageType);

    EffectType ParseEffect(QString& effectDescription, qint32 effectStartPos, EffectsTable* effectsTable, qint32& effectEndPos);
    
};

#endif // TORCHLIGHT2ITEM_H
