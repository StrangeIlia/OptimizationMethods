#ifndef DELEGATECLONER_H
#define DELEGATECLONER_H

#include "SizeController.h"

class ColumnDelegateCloner : public QObject
{
    Q_OBJECT
public:
    ColumnDelegateCloner(SizeController *controller, QObject *parent);
private slots:
    void cloneDelegate(Qt::Orientation orientation, int base, int cl);
public:
    QTableWidget *widget;
};

#endif // DELEGATECLONER_H
