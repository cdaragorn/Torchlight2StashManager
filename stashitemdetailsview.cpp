#include "stashitemdetailsview.h"

#include <QGraphicsTextItem>
#include <QFont>

StashItemDetailsView::StashItemDetailsView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void StashItemDetailsView::SetItemToDisplay(Torchlight2Item item)
{
    item.ParseAdvancedItemDetails(mInfiniteStashLocation);
    QGraphicsScene* newScene = new QGraphicsScene();

    quint32 nextItemYPos = 0;

    QGraphicsTextItem* itemName = new QGraphicsTextItem();

    QFont titleFont("Times New Roman", 20);
    titleFont.setBold(true);
    QFont listFont();

    itemName->setPos(0, nextItemYPos);
    QColor color(255, 255, 255);
    itemName->setDefaultTextColor(color);
    itemName->setPlainText(item.Name());
    itemName->setFont(titleFont);

    nextItemYPos = itemName->boundingRect().height();
    newScene->addItem(itemName);


    qint32 numEmptySockets = item.NumSockets() - item.NumFilledSockets();

    for (qint32 i = 0; i < numEmptySockets; ++i)
    {
        QGraphicsTextItem* nextEmptySocket =
                new QGraphicsTextItem("Empty Socket");
        nextEmptySocket->setDefaultTextColor(color);
        nextEmptySocket->setPos(0, nextItemYPos);
        nextItemYPos += nextEmptySocket->boundingRect().height();
        newScene->addItem(nextEmptySocket);
    }

    for (qint32 i = 0; i < item.NumFilledSockets(); ++i)
    {
        QGraphicsTextItem* nextSocketedItem =
                new QGraphicsTextItem("Socket (" + item.SocketedItems()[i].name + ")");
        nextSocketedItem->setDefaultTextColor(color);
        nextSocketedItem->setPos(0, nextItemYPos);
        nextItemYPos += nextSocketedItem->boundingRect().height();

        newScene->addItem(nextSocketedItem);

        QGraphicsTextItem* socketEffect =
                new QGraphicsTextItem(item.SocketedItems()[i].effect);
        socketEffect->setDefaultTextColor(color);
        socketEffect->setPos(10, nextItemYPos);
        nextItemYPos += socketEffect->boundingRect().height();

        newScene->addItem(socketEffect);
    }

//    QGraphicsTextItem* itemNumSockets =
//            new QGraphicsTextItem("Number of Sockets: " +
//                                  QString::number(item.NumSockets()));

//    itemNumSockets->setDefaultTextColor(color);
//    itemNumSockets->setPos(0, itemName->boundingRect().height());

//    QGraphicsTextItem* itemNumFilledSockets =
//            new QGraphicsTextItem("Number of filled Sockets: " +
//                                  QString::number(item.NumFilledSockets()));

//    itemNumFilledSockets->setDefaultTextColor(color);
//    itemNumFilledSockets->setPos(0, itemNumSockets->y() + itemNumSockets->boundingRect().height());

//    newScene->addItem(itemNumSockets);
//    newScene->addItem(itemNumFilledSockets);

    QGraphicsScene* oldScene = this->scene();
    this->setScene(newScene);

    if (oldScene != NULL)
    {
//        QList<QGraphicsItem*> oldItems = oldScene->items();

//        for (int i = 0; i < oldItems.length(); ++i)
//        {
//            delete oldItems[i];
//        }

        delete oldScene;
    }
}
