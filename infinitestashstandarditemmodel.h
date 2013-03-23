#ifndef INFINITESTASHSTANDARDITEMMODEL_H
#define INFINITESTASHSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>

class InfiniteStashStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit InfiniteStashStandardItemModel(QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    enum ItemType
    {
        Group,
        StashItem
    };
    
signals:
    
public slots:

protected:

    
};

#endif // GROUPMANAGERSTANDARDITEMMODEL_H
