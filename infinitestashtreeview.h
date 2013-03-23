#ifndef INFINITESTASHTREEVIEW_H
#define INFINITESTASHTREEVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItem>

class InfiniteStashTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit InfiniteStashTreeView(QWidget *parent = 0);

    virtual void setModel(QAbstractItemModel *model);

    bool wasDropIndicatorOnItem();

    qint8 whereWasDropIndicator();
    
signals:
    void itemDropped(QDropEvent* event);
    
public slots:
//    void OnItemInsertedToModel(QStandardItem* item);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);

protected:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end);
    
};

#endif // GROUPMANAGERTREEVIEW_H
