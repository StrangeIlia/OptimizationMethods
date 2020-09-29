#ifndef DELEGATEDOUBLE_H
#define DELEGATEDOUBLE_H

#include <QLineEdit>
#include <QApplication>
#include <QStyledItemDelegate>

class DelegateDouble : public QStyledItemDelegate {
    Qt::Alignment flags;
    QDoubleValidator* validator;
public:
    DelegateDouble(Qt::Alignment _flags, QObject* parent = nullptr);
private:
    void paint(QPainter *painter, const QStyleOptionViewItem &_option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override ;
};

#endif // DELEGATEDOUBLE_H
