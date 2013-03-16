#ifndef GROUPMANAGERSTANDARDITEMMODEL_H
#define GROUPMANAGERSTANDARDITEMMODEL_H

#include <QWidget>
#include <QStandardItemModel>

class GroupManagerStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit GroupManagerStandardItemModel(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // GROUPMANAGERSTANDARDITEMMODEL_H
