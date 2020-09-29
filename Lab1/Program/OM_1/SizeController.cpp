#include "SizeController.h"

SizeController::SizeController(QTableWidget *widget, QObject *parent) : QObject(parent)
{
    if(widget == nullptr)
        throw std::invalid_argument("SizeController::SizeController(QTabWidget*, int, int): отслеживаемый объект не может быть пустым");
    _minRowCount = -1;
    _minColumnCount = -1;
    _maxRowCount = -1;
    _maxColumnCount = -1;
    _trackedWidget = widget;
    connect(widget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(addRowAndOrColumn(QTableWidgetItem*)));
    connect(widget->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(removeSelectedRow(int)));
    connect(widget->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(removeSelectedColumn(int)));
}

QTableWidget *SizeController::tractedWidget() const {
    return _trackedWidget;
}

int SizeController::maxRowCount() const {
    return _maxRowCount;
}

int SizeController::maxColumnCount() const {
    return _maxColumnCount;
}

int SizeController::minRowCount() const {
    return _minRowCount;
}

int SizeController::minColumnCount() const {
    return _minColumnCount;
}

void SizeController::setMaxRowCount(int value) {
    if(value == 0)
        throw std::invalid_argument("SizeController::setMaxRowCount(int): максимальное число строк не может быть 0");
    _maxRowCount = value > 0 ? value : -1;
}

void SizeController::setMaxColumnCount(int value) {
    if(value == 0)
        throw std::invalid_argument("SizeController::setMaxColumnCount(int): максимальное число столбцов не может быть 0");
    _maxColumnCount = value > 0 ? value : -1;
}

void SizeController::setMinRowCount(int value) {
    if(value == 0)
         throw std::invalid_argument("SizeController::setMinRowCount(int): минимальное число строк не может быть 0");
    _minRowCount = value > 0 ? value : -1;
}

void SizeController::setMinColumnCount(int value) {
    if(value == 0)
         throw std::invalid_argument("SizeController::setMinColumnCount(int): минимальное число столбцов не может быть 0");
    _minColumnCount = value > 0 ? value : -1;
}

bool SizeController::expendRequired(QHeaderView *headerView, int index, int max) {
    QAbstractItemModel *model = headerView->model();
    QVariant headerInfo = model->headerData(index, headerView->orientation(), HeaderInfo::TypeRole);
    if(headerInfo.isValid()) {
        HeaderInfo header = headerInfo.value<QVariantMap>();
        bool reqExt = false;
        if(!reqExt && (header.extension() & HeaderInfo::Below)) {
            if(index == 0) {
                reqExt = true;
            } else {
                QVariant below = model->headerData(index - 1, headerView->orientation(), HeaderInfo::TypeRole);
                if(below.isValid()) {
                    reqExt = ((HeaderInfo)below.value<QVariantMap>()).group() != header.group();
                } else reqExt = true;
            }
        }
        if(!reqExt && (header.extension() & HeaderInfo::Above)) {
            if(index + 1 == headerView->count()) {
                reqExt = true;
            } else {
                QVariant above = model->headerData(index + 1, headerView->orientation(), HeaderInfo::TypeRole);
                if(above.isValid()) {
                    reqExt = ((HeaderInfo)above.value<QVariantMap>()).group() != header.group();
                } else reqExt = true;
            }
        }
        if(reqExt) {
            int counts = 0;
            for(int i = 0; i != headerView->count(); ++i) {
                QVariant variant = model->headerData(i, headerView->orientation(), HeaderInfo::TypeRole);
                if(variant.isValid()) {
                    if(((HeaderInfo)variant.value<QVariantMap>()).group() == header.group()) {
                        ++counts;
                    }
                }

            }
            if(max == -1 || counts < max) {
                return true;
            }
        }
    }
    return false;
}

bool SizeController::removeRequired(QHeaderView *headerView, int index, int min) {
    QAbstractItemModel *model = headerView->model();
    QVariant headerInfo = model->headerData(index, headerView->orientation(), HeaderInfo::TypeRole);
    if(headerInfo.isValid()) {
        HeaderInfo header = headerInfo.value<QVariantMap>();
        if(header.deletion()) {
            int counts = 0;
            for(int i = 0; i != headerView->count(); ++i) {
                QVariant variant = model->headerData(index, headerView->orientation(), HeaderInfo::TypeRole);
                if(variant.isValid()) {
                    if(((HeaderInfo)variant.value<QVariantMap>()).deletion()) {
                        ++counts;
                    }
                }
            }
            if(counts > min) {
                return true;
            }
        }
    }
    return false;
}

void SizeController::addRowAndOrColumn(QTableWidgetItem *item) {
    if(expendRequired(_trackedWidget->verticalHeader(), item->row(), _maxRowCount)) {
        HeaderInfo info = _trackedWidget->verticalHeaderItem(item->row())->data(HeaderInfo::TypeRole).value<QVariantMap>();
        if(info.extension() & HeaderInfo::Above){
            _trackedWidget->insertRow(item->row() + 1);
            _trackedWidget->verticalHeaderItem(item->row() + 1)->setData(HeaderInfo::TypeRole, info);
            emit cloneRowOrColumn(Qt::Orientation::Vertical, item->row(), item->row() + 1);
        }
        if(info.extension() & HeaderInfo::Below) {
            _trackedWidget->insertRow(item->row());
            _trackedWidget->verticalHeaderItem(item->row())->setData(HeaderInfo::TypeRole, info);
            emit cloneRowOrColumn(Qt::Orientation::Vertical, item->row() + 1, item->row());
        }
    }
    if(expendRequired(_trackedWidget->horizontalHeader(), item->column(), _maxColumnCount)) {
        HeaderInfo info = _trackedWidget->horizontalHeaderItem(item->column())->data(HeaderInfo::TypeRole).value<QVariantMap>();
        if(info.extension() & HeaderInfo::Above) {
            _trackedWidget->insertColumn(item->column() + 1);
            _trackedWidget->setHorizontalHeaderItem(item->column() + 1, new QTableWidgetItem());
            _trackedWidget->horizontalHeaderItem(item->column() + 1)->setData(HeaderInfo::TypeRole, info);
            emit cloneRowOrColumn(Qt::Orientation::Horizontal, item->column(), item->column() + 1);
        }
        if(info.extension() & HeaderInfo::Below) {
            _trackedWidget->insertColumn(item->column());
            _trackedWidget->setHorizontalHeaderItem(item->column(), new QTableWidgetItem());
            _trackedWidget->horizontalHeaderItem(item->column())->setData(HeaderInfo::TypeRole, info);
            emit cloneRowOrColumn(Qt::Orientation::Horizontal, item->column() + 1, item->column());
        }
    }
}

void SizeController::removeSelectedRow(int row) {
    if(removeRequired(_trackedWidget->verticalHeader(), row, _minRowCount))
        _trackedWidget->removeRow(row);
}

void SizeController::removeSelectedColumn(int column) {
    if(removeRequired(_trackedWidget->horizontalHeader(), column, _minColumnCount))
        _trackedWidget->removeColumn(column);
}
