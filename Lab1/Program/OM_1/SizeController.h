#ifndef SIZECONTROLLER_H
#define SIZECONTROLLER_H

#include <stdexcept>
#include <QHeaderView>
#include <QTableWidget>

#include "HeaderInfo.h"

/// Ориентируется на разрешения в заголовке
/// Добавляет элемент (строку или столбец), если есть разрешение
/// и группа соседнего элемента, находящийся в стороне расширения, отличается
/// При добавлении копирует информацию в заголовке
class SizeController : public QObject
{
    Q_OBJECT
public:
    SizeController(QTableWidget *widget, QObject *parent = nullptr);

    QTableWidget *tractedWidget() const;

    int maxRowCount() const;
    int maxColumnCount() const;
    int minRowCount() const;
    int minColumnCount() const;

    void setMaxRowCount(int value);
    void setMaxColumnCount(int value);
    void setMinRowCount(int value);
    void setMinColumnCount(int value);

signals:
    void cloneRowOrColumn(Qt::Orientation orientation, int base, int cl);

public slots:
    void addRowAndOrColumn(QTableWidgetItem *item);
    void removeSelectedRow(int row);
    void removeSelectedColumn(int column);

private:
    static bool expendRequired(QHeaderView *header, int index, int max);
    static bool removeRequired(QHeaderView *header, int index, int min);

    int _maxRowCount, _maxColumnCount;
    int _minRowCount, _minColumnCount;
    QTableWidget *_trackedWidget;
};

#endif // SIZECONTROLLER_H
