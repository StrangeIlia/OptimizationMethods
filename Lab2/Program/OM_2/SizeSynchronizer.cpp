#include "SizeSynchronizer.h"

#include <QMetaMethod>

SizeSynchronizer::SizeSynchronizer(QAbstractItemModel *master, QAbstractItemModel *slave,
                                   bool _synchRows, bool _synchColumns, QObject *parent) : QObject(parent) {
    if(master == nullptr || slave == nullptr)
        throw std::invalid_argument("SizeSynchronizer::SizeSynchronizer(QTableWidget*, QTableWidget*, bool, bool, QObject*): Для синхронизации нужно две модели");
//    if(master->rowCount() != slave->rowCount())
//        throw std::invalid_argument("SizeSynchronizer::SizeSynchronizer(QTableWidget*, QTableWidget*, bool, bool, QObject*): Перед синхронизацией число строк в моделях должно совпадать");
//    if(master->columnCount() != slave->columnCount())
//        throw std::invalid_argument("SizeSynchronizer::SizeSynchronizer(QTableWidget*, QTableWidget*, bool, bool, QObject*): Перед синхронизацией число столбцов в моделях должно совпадать");
    _masterModel = master;
    _slaveModel = slave;

    /// Тестирование
//    {
//        auto metaObject = _firstModel->metaObject();
//        for(int i = 0; i != metaObject->methodCount(); ++i) {
//            auto metaMethod = metaObject->method(i);
//            if(metaMethod.methodType() == QMetaMethod::MethodType::Signal){
//                auto str = metaMethod.name().toStdString() + "(";
//                for(auto parameter : metaMethod.parameterTypes()) {
//                    str += parameter.toStdString() + ",";
//                }
//                str += ")";

//                qInfo(str.data());
//            }
//        }
//    }

    synchRows = false;
    synchColumns = false;
    rowsSynchonize(_synchRows);
    columnsSynchonize(_synchColumns);
}

QAbstractItemModel *SizeSynchronizer::masterModel() {
    return _masterModel;
}

void SizeSynchronizer::changeMasterModel(QAbstractItemModel *newFirstModel) {
    bool rows = synchRows, column = synchColumns;
    rowsSynchonize(false);
    columnsSynchonize(false);
    _masterModel = newFirstModel;
    rowsSynchonize(rows);
    columnsSynchonize(column);
}

QAbstractItemModel *SizeSynchronizer::slaveModel() {
    return _slaveModel;
}

void SizeSynchronizer::changeSlaveModel(QAbstractItemModel *newSecondModel) {
    bool rows = synchRows, column = synchColumns;
    rowsSynchonize(false);
    columnsSynchonize(false);
    _slaveModel = newSecondModel;
    rowsSynchonize(rows);
    columnsSynchonize(column);
}

bool SizeSynchronizer::rowsSynchonized() const {
    return synchRows;
}

void SizeSynchronizer::rowsSynchonize(bool value) {
    if(synchRows == value) return;
    synchRows = value;
    if(synchRows) {
        connect(_masterModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(decreaseRowCount(const QModelIndex&, int, int)));
        connect(_masterModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(increaseRowCount(const QModelIndex&, int, int)));
    } else  {
        disconnect(_masterModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(decreaseRowCount(const QModelIndex&, int, int)));
        disconnect(_masterModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(increaseRowCount(const QModelIndex&, int, int)));
    }
}

bool SizeSynchronizer::columnsSynchonized() const {
    return synchColumns;
}

void SizeSynchronizer::columnsSynchonize(bool value) {
    if(synchColumns == value) return;
    synchColumns = value;
    if(synchColumns) {
        connect(_masterModel, SIGNAL(columnsRemoved(const QModelIndex&, int, int)), this, SLOT(decreaseColumnCount(const QModelIndex&, int, int)));
        connect(_masterModel, SIGNAL(columnsInserted(const QModelIndex&, int, int)), this, SLOT(increaseColumnCount(const QModelIndex&, int, int)));
    } else {
        disconnect(_masterModel, SIGNAL(columnsRemoved(const QModelIndex&, int, int)), this, SLOT(decreaseColumnCount(const QModelIndex&, int, int)));
        disconnect(_masterModel, SIGNAL(columnsInserted(const QModelIndex&, int, int)), this, SLOT(increaseColumnCount(const QModelIndex&, int, int)));
    }
}

void SizeSynchronizer::increaseRowCount(const QModelIndex&, int first, int last) {
    if(_masterModel->rowCount() != _slaveModel->rowCount())
        _slaveModel->insertRows(first, last - first + 1);
}

void SizeSynchronizer::increaseColumnCount(const QModelIndex&, int first, int last) {
    if(_masterModel->columnCount() != _slaveModel->columnCount())
        _slaveModel->insertColumns(first, last - first + 1);
}

void SizeSynchronizer::decreaseRowCount(const QModelIndex&, int first, int last) {
    if(_masterModel->rowCount() != _slaveModel->rowCount())
        _slaveModel->removeRows(first, last - first + 1);
}

void SizeSynchronizer::decreaseColumnCount(const QModelIndex&, int first, int last) {
    if(_masterModel->columnCount() != _slaveModel->columnCount())
        _slaveModel->removeColumns(first, last - first + 1);
}
