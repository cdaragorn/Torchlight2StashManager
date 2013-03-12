#ifndef TORCHLIGHT2ITEM_H
#define TORCHLIGHT2ITEM_H

#include <QByteArray>
#include <QString>

class Torchlight2Item/* : public QObject*/
{
//    Q_OBJECT
public:
    explicit Torchlight2Item(QByteArray itemBytes/*, QObject *parent = 0*/);
    ~Torchlight2Item();

    QByteArray Bytes() { return mItemBytes; }
    QString Name() { return mItemName; }
    
//signals:
    
//public slots:

private:
    QByteArray mItemBytes;

    qint64 mItemUniqueTypeId;
    QString mItemName;

    void ParseItemDetails();

    
};

#endif // TORCHLIGHT2ITEM_H
