#include "SystemOfRestrictions.h"

#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QApplication>
#include <QDoubleValidator>

//class DelegateDouble : public QStyledItemDelegate {
//    Qt::Alignment flags;
//    QDoubleValidator* validator;
//public:
//    DelegateDouble(Qt::Alignment _flags, QObject* parent = nullptr);

//    void paint(QPainter *painter, const QStyleOptionViewItem &_option, const QModelIndex &index) const override {
//        QStyleOptionViewItem option = _option;
//        initStyleOption(&option, index);
//        option.displayAlignment = flags;

//        QStyle *style = QApplication::style();
//        style->drawControl(QStyle::CE_ItemViewItem, &option, painter);
//    }

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override {
//        QLineEdit* tableItem = new QLineEdit(parent);
//        tableItem->setText("0,0"); // Почему - то не работает
//        tableItem->setValidator(validator);
//        return tableItem;
//    }
//};

//class DelegateComboBox : public QStyledItemDelegate {
//public:
//    DelegateComboBox(QObject* parent = nullptr) : QStyledItemDelegate(parent) {

//    }

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem&, const QModelIndex&) const override {
//        QComboBox* tableItem = new QComboBox(parent);
//        tableItem->addItem("=", QVariant::fromValue(0));
//        tableItem->addItem("<=", QVariant::fromValue(1));
//        tableItem->addItem(">=", QVariant::fromValue(2));
//        tableItem->setCurrentIndex(0);
//        return tableItem;
//    }
//};

SystemOfRestrictions::SystemOfRestrictions(QWidget *parent) : QTableWidget(parent) {
    auto header = horizontalHeader();
    header->setSectionsMovable(false);
    header->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    connect(header, SIGNAL(sectionDoubleClicked(int)), this, SLOT(removeSelectedColumn(int)));
    header = verticalHeader();
    header->setSectionsMovable(false);
    connect(header, SIGNAL(sectionDoubleClicked(int)), this, SLOT(removeSelectedRow(int)));

    delegateDouble = new DelegateDouble(Qt::AlignRight | Qt::AlignVCenter, this);
    delegateComboBox = new DelegateComboBox(this);

    addVariable();
    insertColumnHeader(1, "Отношение", delegateComboBox);
    insertColumnHeader(2, "b", delegateDouble);
    addRow();

    connect(this, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(checkUpdate(QTableWidgetItem*)));
}

QVector<SystemOfRestrictions::Signs> SystemOfRestrictions::getSigns() {
    QVector<Signs> result;
    for(int i = 0; i != system.countRows; ++i) {
        QComboBox* tableItem = (QComboBox*) item(i, columnCount() - 2);
        int index = tableItem->currentData(0).value<int>();
        result.push_back(Signs(index));
    }
    return result;
}

MatrixOnRow<double> SystemOfRestrictions::getFreeMembers() const {
    MatrixOnRow<double> result(system.countRows, 1);
    for(int i = 0; i != system.countRows; ++i) {
        QLineEdit* tableItem = (QLineEdit*) item(i, columnCount() - 1);
        result(i, 0) = tableItem->text().toDouble();
    }
    return result;
}

MatrixOnRow<double> SystemOfRestrictions::getCoefficients() const {
    MatrixOnRow<double> result(system.countRows, system.countColumns);
    for(int i = 0; i != system.countRows; ++i) {
        for(int j = 0; j != system.countColumns; ++j) {
            QLineEdit* tableItem = (QLineEdit*) item(i, columnCount() - 1);
            result(i, 0) = tableItem->text().toDouble();
        }
    }
    return result;
}

void SystemOfRestrictions::addRow() {
    insertRowHeader(rowCount(), QString::number(rowCount() + 1));
    ++system.countRows;
}

void SystemOfRestrictions::addVariable() {
    insertColumnHeader(system.countColumns, "x" + QString::number(system.countColumns + 1), delegateDouble);
    ++system.countColumns;
}

void SystemOfRestrictions::insertRowHeader(int index, QString value, QStyledItemDelegate* delegate) {
    insertRow(index);
    setVerticalHeaderItem(index, new QTableWidgetItem(value));
    if(delegate != nullptr)
        setItemDelegateForRow(index, delegate);
}

void SystemOfRestrictions::insertColumnHeader(int index, QString value, QStyledItemDelegate* delegate) {
    insertColumn(index);
    setHorizontalHeaderItem(index, new QTableWidgetItem(value));
    if(delegate != nullptr)
        setItemDelegateForColumn(index, delegate);
}


void SystemOfRestrictions::checkUpdate(QTableWidgetItem* i) {
    if(i->text() != "") {
        if(i->row() + 1 == system.countRows) addRow();
        if(i->column() + 1 == system.countColumns) addVariable();
    }
}

void SystemOfRestrictions::removeSelectedRow(int row) {
    if(rowCount() != 1) {
        removeRow(row);
        --system.countRows;
        for(int i = 0; i != rowCount(); ++i){
            setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i + 1)));
        }
    }
}

void SystemOfRestrictions::removeSelectedColumn(int column) {
    if(system.countColumns != 1 && column < system.countColumns) {
        removeColumn(column);
        --system.countColumns;
        for(int i = 0; i != system.countColumns; ++i){
            setHorizontalHeaderItem(i, new QTableWidgetItem("x" + QString::number(i + 1)));
        }
    }
}
