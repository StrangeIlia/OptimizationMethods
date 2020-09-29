#include "DelegateComboBox.h"

DelegateComboBox::DelegateComboBox(QStringList list, QObject* parent) : QStyledItemDelegate(parent) {
    for(auto str : list) hash.insert(str, QVariant());
}

DelegateComboBox::DelegateComboBox(QVariantHash hash, QObject* parent) : QStyledItemDelegate(parent) {
    this->hash = hash;
}

QWidget *DelegateComboBox::createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const {
    QComboBox* tableItem = new QComboBox(parent);
    for(auto i = hash.begin(); i != hash.end(); ++i) {
        tableItem->addItem(i.key(), i.value());
    }
    tableItem->setCurrentIndex(0);
    return tableItem;
}
