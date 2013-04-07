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
    QGraphicsScene* scene = new QGraphicsScene();

    QGraphicsTextItem* itemName = new QGraphicsTextItem();

    QFont titleFont("Times New Roman", 20);
    titleFont.setBold(true);
    QFont listFont();

    itemName->setPos(0, 0);
    QColor color(255, 255, 255);
    itemName->setDefaultTextColor(color);
    itemName->setPlainText(item.Name());
    itemName->setFont(titleFont);

    QGraphicsTextItem* itemNumSockets =
            new QGraphicsTextItem("Number of Sockets: " +
                                  QString::number(item.NumSockets()));

    itemNumSockets->setDefaultTextColor(color);
    itemNumSockets->setPos(0, itemName->boundingRect().height());

    QGraphicsTextItem* itemNumFilledSockets =
            new QGraphicsTextItem("Number of filled Sockets: " +
                                  QString::number(item.NumFilledSockets()));

    itemNumFilledSockets->setDefaultTextColor(color);
    itemNumFilledSockets->setPos(0, itemNumSockets->y() + itemNumSockets->boundingRect().height());

    scene->addItem(itemName);
    scene->addItem(itemNumSockets);
    scene->addItem(itemNumFilledSockets);

//    scene->setSceneRect(this->geometry());

    this->setScene(scene);
    this->show();
}
