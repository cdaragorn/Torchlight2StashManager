#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H

#include <QListWidget>
#include <QList>
#include <QListWidgetItem>

class CustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CustomListWidget(QWidget *parent = 0);
    
protected:
    virtual void rowsInserted(const QModelIndex &parent, int start, int end);
    virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

signals:
    void itemAdded(QListWidgetItem* item);
    void itemsAboutToBeRemoved(QList<QListWidgetItem*> items);
    
public slots:
    
};

#endif // CUSTOMLISTWIDGET_H
