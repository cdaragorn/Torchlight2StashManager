#ifndef STASHITEMDETAILSVIEW_H
#define STASHITEMDETAILSVIEW_H

#include <QWidget>
#include <QGraphicsView>

#include <torchlight2item.h>

class StashItemDetailsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit StashItemDetailsView(QWidget *parent = 0);

    void SetItemToDisplay(Torchlight2Item item);
    
signals:
    
public slots:
    
};

#endif // STASHITEMDETAILSVIEW_H
