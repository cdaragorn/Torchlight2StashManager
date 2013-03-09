#ifndef TORCHLIGHT2STASH_H
#define TORCHLIGHT2STASH_H

#include <QObject>
#include <QHash>

class Torchlight2Stash : public QObject
{
    Q_OBJECT
public:
    explicit Torchlight2Stash(QByteArray& sharedStash, QObject *parent = 0);
    
signals:
    
public slots:

private:
    QByteArray mStash;

    QHash<QString, QByteArray> mItemsInStash;

    void ReadItemsInStash();
    
};

#endif // TORCHLIGHT2STASH_H
