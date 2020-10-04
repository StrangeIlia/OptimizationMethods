#ifndef LABELHEADERVIEW_H
#define LABELHEADERVIEW_H

#include <QPainter>
#include <QHeaderView>
#include <QTextDocument>

class LabelHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    LabelHeaderView(Qt::Orientation orientation,/* Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter,*/ QWidget *parent = nullptr);
//    Qt::Alignment alignment() const;
//    void setAlignment(Qt::Alignment alignment);
private:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
//    Qt::Alignment _alignment;
};

#endif // LABELHEADERVIEW_H
