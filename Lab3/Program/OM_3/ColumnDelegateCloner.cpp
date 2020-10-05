#include "ColumnDelegateCloner.h"

ColumnDelegateCloner::ColumnDelegateCloner(SizeController *controller, QObject *parent) : QObject(parent)
{
    connect(controller, &SizeController::cloneRowOrColumn, this, &ColumnDelegateCloner::cloneDelegate);
    widget = controller->tractedWidget();
}

void ColumnDelegateCloner::cloneDelegate(Qt::Orientation orientation, int base, int cl) {
    if(orientation == Qt::Orientation::Horizontal)
        widget->setItemDelegateForColumn(cl, widget->itemDelegateForColumn(base));
}
