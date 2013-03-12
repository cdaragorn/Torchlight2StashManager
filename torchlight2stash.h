#ifndef TORCHLIGHT2STASH_H
#define TORCHLIGHT2STASH_H

#include <QObject>
#include <QHash>
#include <torchlight2item.h>

class Torchlight2Stash : public QObject
{
    Q_OBJECT
public:
    explicit Torchlight2Stash(QByteArray& sharedStash, QObject *parent = 0);
    
    QList<Torchlight2Item> StashItems() { return mItemsInStash; }
signals:
    
public slots:

private:
    QByteArray mStash;

    QList<Torchlight2Item> mItemsInStash;

    void ReadItemsInStash();
    
};

#endif // TORCHLIGHT2STASH_H
