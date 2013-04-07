#ifndef SHAREDSTASHLISTWIDGET_H
#define SHAREDSTASHLISTWIDGET_H

#include <QListWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QAction>
#include <QMessageBox>
#include <QList>
#include <sstream>
#include <customlistwidget.h>

class SharedStashListWidget : public CustomListWidget
{
    Q_OBJECT
public:
    explicit SharedStashListWidget(QWidget *parent = 0);
    ~SharedStashListWidget();

    QListWidgetItem* GetDroppedItem(QPoint inPosition);
    DropIndicatorPosition dropPosition() { return dropIndicatorPosition(); }
    QListWidgetItem* ItemAtIndex(QModelIndex index) { return itemFromIndex(index); }

protected slots:
    virtual void contextMenuEvent(QContextMenuEvent * inEvent);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void itemChanged(QListWidgetItem *item);
    virtual void dropEvent(QDropEvent *event);

private:
    QMenu* mContextMenu;
    
signals:
    void itemDropped(QDropEvent*);
    void dragEntered(QDragEnterEvent* event);

    
public slots:

protected slots:
    virtual void dragEnterEvent(QDragEnterEvent *event);

private slots:
    void OnAction();

};

#endif // SHAREDSTASHLISTWIDGET_H
