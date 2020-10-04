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
        tableItem->insertItem(0, i.key(), i.value());
    }
    tableItem->setCurrentIndex(0);
    return tableItem;
}

void DelegateComboBox::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
    if(comboBox == nullptr) return;
    QModelIndex model_index = index;
    if(index.isValid()) {
        auto index = comboBox->currentIndex();
        model->setData(model_index, comboBox->itemText(index), Qt::EditRole);
        model->setData(model_index, comboBox->itemData(index), Qt::UserRole);
    }
}
