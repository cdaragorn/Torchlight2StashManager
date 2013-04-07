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
    qint32 NumSockets() const { return mNumSockets; }
    qint32 NumFilledSockets() const { return mNumFilledSockets; }

    void SetIndex(quint16 index);
    qint16 Index();
    qint32 GetItemNameEndPosition() const { return mItemNameEndPos; }
    qint64 Get24ffBlockEndPos() const { return m24ffBlockEndPos; }

    bool operator ==(const Torchlight2Item& other) const;
    
//signals:
    
//public slots:

private:
    QByteArray mItemBytes;

    qint64 mItemUniqueTypeId;
    QString mItemName;
    qint32 mNumSockets;
    qint32 mNumFilledSockets;

    qint32 mItemNameEndPos;
    qint64 m24ffBlockEndPos;

    void ParseItemDetails();

    
};

#endif // TORCHLIGHT2ITEM_H
