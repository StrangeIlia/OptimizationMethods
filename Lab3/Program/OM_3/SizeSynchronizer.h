#ifndef SIZESYNCHRONIZER_H
#define SIZESYNCHRONIZER_H

#include <QHeaderView>
#include <QTableWidget>

/// Синхронизирует размер модели slave с размером модели master
/// Перед синхронизацией размеры моделей должны совпадать
class SizeSynchronizer : public QObject
{
    Q_OBJECT
private:
    QAbstractItemModel *_masterModel;
    QAbstractItemModel *_slaveModel;
    bool synchRows, synchColumns;
public:
    SizeSynchronizer(QAbstractItemModel *master, QAbstractItemModel *slave,
                     bool synchRows = true, bool synchColumns = true, QObject *parent = nullptr);

    QAbstractItemModel *masterModel();
    void changeMasterModel(QAbstractItemModel *newFirstModel);

    QAbstractItemModel *slaveModel();
    void changeSlaveModel(QAbstractItemModel *newSecondModel);

    bool rowsSynchonized() const;
    void rowsSynchonize(bool value);

    bool columnsSynchonized() const;
    void columnsSynchonize(bool value);

private slots:
    void increaseRowCount(const QModelIndex& parent, int first, int last);
    void increaseColumnCount(const QModelIndex& parent, int first, int last);
    void decreaseRowCount(const QModelIndex& parent, int first, int last);
    void decreaseColumnCount(const QModelIndex& parent, int first, int last);
};

#endif // SIZESYNCHRONIZER_H
