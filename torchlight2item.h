#ifndef TORCHLIGHT2ITEM_H
#define TORCHLIGHT2ITEM_H

#include <QByteArray>
#include <QString>

class Torchlight2Item
{

public:
    explicit Torchlight2Item(QByteArray itemBytes);
    ~Torchlight2Item();

    QByteArray Bytes() const { return mItemBytes; }
    QString Name() const { return mItemName; }

    void SetIndex(quint16 index);
    qint32 GetItemNameEndPosition() const { return mItemNameEndPos; }

    bool operator ==(const Torchlight2Item& other) const;
    
//signals:
    
//public slots:

private:
    QByteArray mItemBytes;

    qint64 mItemUniqueTypeId;
    QString mItemName;

    qint32 mItemNameEndPos;

    void ParseItemDetails();

    
};

#endif // TORCHLIGHT2ITEM_H
