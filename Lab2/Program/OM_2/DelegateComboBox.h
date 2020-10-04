#ifndef DELEGATECOMBOBOX_H
#define DELEGATECOMBOBOX_H

#include <QComboBox>
#include <QStringList>
#include <QVariantHash>
#include <QStyledItemDelegate>


class DelegateComboBox : public QStyledItemDelegate {
public:
    DelegateComboBox(QStringList list, QObject* parent = nullptr);
    DelegateComboBox(QVariantHash hash, QObject* parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private:
    QVariantHash hash;
};

#endif // DELEGATECOMBOBOX_H
