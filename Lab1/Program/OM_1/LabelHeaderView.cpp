#include "LabelHeaderView.h"

LabelHeaderView::LabelHeaderView(Qt::Orientation orientation, /*Qt::Alignment alignment, */QWidget *parent) : QHeaderView(orientation, parent) {
//    _alignment = alignment;
}

//Qt::Alignment LabelHeaderView::alignment() const {
//    return _alignment;
//}

//void LabelHeaderView::setAlignment(Qt::Alignment alignment) {
//    _alignment = alignment;
//}

void LabelHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const {
    if(!rect.isValid()) return;

    QStyleOptionHeader opt;
    initStyleOption(&opt);
    opt.rect = rect;
    opt.section = logicalIndex;
    opt.textAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    int visual = visualIndex(logicalIndex);

    if(count() == 1)
        opt.position = QStyleOptionHeader::OnlyOneSection;
    else if(visual == 0)
        opt.position = QStyleOptionHeader::Beginning;
    else if(visual + 1 == count())
        opt.position = QStyleOptionHeader::End;
    else
        opt.position = QStyleOptionHeader::Middle;

    style()->drawControl(QStyle::CE_Header, &opt, painter, this);

    painter->save();
    QRect textRect = style()->subElementRect(QStyle::SE_HeaderLabel, &opt, this);
    painter->translate(textRect.topLeft());
    QTextDocument doc;
    //doc.setDocumentMargin(0);
    doc.setHtml(model()->headerData(logicalIndex, orientation()).toString());
//    doc.setDefaultTextOption(_alignment);
    doc.drawContents(painter, QRect(QPoint(0, 0), textRect.size()));
    painter->restore();
}
