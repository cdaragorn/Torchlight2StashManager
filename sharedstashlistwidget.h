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

protected:
    virtual void contextMenuEvent(QContextMenuEvent * inEvent);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void itemChanged(QListWidgetItem *item);
    virtual void dropEvent(QDropEvent *event);

private:
    QMenu* mContextMenu;
    
signals:

    
public slots:

private slots:
    void OnAction();

};

#endif // SHAREDSTASHLISTWIDGET_H
