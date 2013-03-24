#ifndef GROUPMANAGERTREEVIEW_H
#define GROUPMANAGERTREEVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QMenu>
#include <groupstable.h>
#include <QStandardItem>

class GroupManagerTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit GroupManagerTreeView(QWidget *parent = 0);
    ~GroupManagerTreeView();

    
signals:
    void groupDeleted(qint64);
    
public slots:

protected:
    virtual void contextMenuEvent(QContextMenuEvent * inEvent);

private slots:
    void OnDeleteGroupClicked();

private:
    QMenu* mContextMenu;

    
};

#endif // GROUPMANAGERTREEVIEW_H
