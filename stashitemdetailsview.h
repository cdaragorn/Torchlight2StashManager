#ifndef STASHITEMDETAILSVIEW_H
#define STASHITEMDETAILSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QDir>

#include <torchlight2item.h>

class StashItemDetailsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StashItemDetailsView(QWidget *parent = 0);

    void SetItemToDisplay(Torchlight2Item item);

    void InfiniteStashLocation(QString infiniteStashFolder)
    {
        mInfiniteStashLocation.setPath(infiniteStashFolder);
    }
    
signals:
    
public slots:

private:
    QDir mInfiniteStashLocation;
    
};

#endif // STASHITEMDETAILSVIEW_H
