#include "delegate_double.h"

DelegateDouble::DelegateDouble(Qt::Alignment _flags, QObject* parent) :
    QStyledItemDelegate(parent), flags(_flags), validator(new QDoubleValidator(this)) {

}

void DelegateDouble::paint(QPainter *painter, const QStyleOptionViewItem &_option, const QModelIndex &index) const {
    QStyleOptionViewItem option = _option;
    initStyleOption(&option, index);
    option.displayAlignment = flags;

    QStyle *style = QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &option, painter);
}

QWidget* DelegateDouble::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const {
    QLineEdit* tableItem = new QLineEdit(parent);
    tableItem->setText("0,0"); // Почему - то не работает
    tableItem->setValidator(validator);
    return tableItem;
}
