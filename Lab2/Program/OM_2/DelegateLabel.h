#ifndef DELEGATELABEL_H
#define DELEGATELABEL_H

#include <QLabel>
#include <QApplication>
#include <QStyledItemDelegate>

class DelegateLabel : public QStyledItemDelegate
{
public:
    DelegateLabel(QObject* parent = nullptr);
private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override;
};

#endif // DELEGATELABEL_H
