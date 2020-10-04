#include "DelegateLabel.h"

DelegateLabel::DelegateLabel(QObject* parent) : QStyledItemDelegate(parent) {

}

QWidget* DelegateLabel::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const {
    QLabel* tableItem = new QLabel(parent);
    return tableItem;
}
