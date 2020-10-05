#ifndef COLUMNNAMECHANGER_H
#define COLUMNNAMECHANGER_H

#include "HeaderInfo.h"
#include "AbstractNameGiver.hpp"

#include <QAbstractTableModel>

/// Изменяет названия столбцов согласно информации в заголовке
/// Для своей работы использует AbstractNameGiver
class ColumnNameChanger : public QObject
{
    Q_OBJECT
public:
    ColumnNameChanger(QAbstractItemModel *model, QObject *parent);
private slots:
    void nameChange(Qt::Orientation orientation, int first, int last);
private:
    QAbstractItemModel *trackedModel;
public:
    /// Вид поля, с которым работает данный класс
    /// HeaderField : AbstractNameGiverPtr
    /// Название поле, на которое ориентируется данный класс
    static const QString HeaderField;
};

#endif // COLUMNNAMECHANGER_H
