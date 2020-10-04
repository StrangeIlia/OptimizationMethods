#include "DelegateController.h"

DelegateController::DelegateController(QTableWidget *widget, QObject *parent) : QObject(parent) {
    _trackedWidget = widget;
    connect(_trackedWidget->model(), &QAbstractItemModel::rowsInserted, this, &DelegateController::rowsInserted);
    connect(_trackedWidget->model(), &QAbstractItemModel::columnsInserted, this, &DelegateController::columnsInserted);
    connect(_trackedWidget->model(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &DelegateController::rowsRemoved);
    connect(_trackedWidget->model(), &QAbstractItemModel::columnsAboutToBeRemoved, this, &DelegateController::columnsRemoved);
}

void DelegateController::rowsInserted(const QModelIndex &/*parent*/, int first, int last) {
    int shift = last - first + 1;
    for(int i = _trackedWidget->rowCount() - 1; i > last; --i) {
        auto delegate = _trackedWidget->itemDelegateForRow(i - shift);
        _trackedWidget->setItemDelegateForRow(i - shift, nullptr);
        _trackedWidget->setItemDelegateForRow(i, delegate);
    }
}

void DelegateController::rowsRemoved(const QModelIndex &/*parent*/, int first, int last) {
    int shift = last - first + 1;
    for(int i = first; i <= last; ++i) {
        auto delegate = _trackedWidget->itemDelegateForRow(i + shift);
        _trackedWidget->setItemDelegateForRow(i + shift, nullptr);
        _trackedWidget->setItemDelegateForRow(i, delegate);
    }
}

void DelegateController::columnsInserted(const QModelIndex &/*parent*/, int first, int last) {
    int shift = last - first + 1;
    for(int i = _trackedWidget->columnCount() - 1; i > last; --i) {
        auto delegate = _trackedWidget->itemDelegateForColumn(i - shift);
        _trackedWidget->setItemDelegateForColumn(i - shift, nullptr);
        _trackedWidget->setItemDelegateForColumn(i, delegate);
    }
}

void DelegateController::columnsRemoved(const QModelIndex &/*parent*/, int first, int last) {
    int shift = last - first + 1;
    for(int i = first; i <= last; ++i) {
        auto delegate = _trackedWidget->itemDelegateForColumn(i + shift);
        _trackedWidget->setItemDelegateForColumn(i + shift, nullptr);
        _trackedWidget->setItemDelegateForColumn(i, delegate);
    }
}
