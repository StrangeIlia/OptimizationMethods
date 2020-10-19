#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initTables();
    initComboboxes();
}

MainWindow::~MainWindow()
{
    delete ui;
    close();
}

bool MainWindow::checkInputData() const {
    auto model = ui->comsumers->model();
    for(int i = 0; i != model->columnCount(); ++i) {
        auto variant = model->data(model->index(0, i));
        if(!variant.canConvert<double>()) {
            printText(tr("Не все точки поставок имеют значение!!!"));
            return false;
        }
        double value = variant.value<double>();
        if(value < std::numeric_limits<double>::epsilon()) {
            printText(tr("Число поставок не может быть отрицательным"));
            return false;
        }
    }

    model = ui->suppliers->model();
    for(int i = 0; i != model->columnCount(); ++i) {
        auto variant = model->data(model->index(0, i));
        if(!variant.canConvert<double>()) {
            printText(tr("Не все точки потребления имеют значение!!!"));
            return false;
        }
        double value = variant.value<double>();
        if(value < std::numeric_limits<double>::epsilon()) {
            printText(tr("Число потребления не может быть отрицательным"));
            return false;
        }
    }

    model = ui->suppliers->model();
    for(int i = 0; i != model->columnCount(); ++i) {
        auto variant = model->data(model->index(0, i));
        if(variant.canConvert<double>()) {
            double value = variant.value<double>();
            if(value < -std::numeric_limits<double>::epsilon()) {
                printText(tr("Стоимость перевозки не может быть отрицательным числом"));
                return false;
            }
        }
    }

    return true;
}

void MainWindow::initTables() {
    connect(ui->suppliers->model(), &QAbstractItemModel::columnsInserted, [this] (const QModelIndex &parent, int first, int last) {
        ui->costs->model()->insertRows(first, last - first + 1, parent);
    });

    connect(ui->comsumers->model(), &QAbstractItemModel::columnsInserted, [this] (const QModelIndex &parent, int first, int last) {
        ui->costs->model()->insertColumns(first, last - first + 1, parent);
    });

    connect(ui->suppliers->model(), &QAbstractItemModel::columnsRemoved, [this] (const QModelIndex &parent, int first, int last) {
        ui->costs->model()->removeRows(first, last - first + 1, parent);
    });

    connect(ui->comsumers->model(), &QAbstractItemModel::columnsRemoved, [this] (const QModelIndex &parent, int first, int last) {
        ui->costs->model()->removeColumns(first, last - first + 1, parent);
    });

    connect(ui->addSupplier, &QPushButton::clicked, [this] (bool /*ignored*/) {
        ui->suppliers->insertColumn(ui->suppliers->columnCount() - 1);
    });

    connect(ui->addComsumer, &QPushButton::clicked, [this] (bool /*ignored*/) {
        ui->comsumers->insertColumn(ui->comsumers->columnCount() - 1);
    });

    connect(ui->suppliers->horizontalHeader(), &QHeaderView::sectionDoubleClicked, [this] (int logicalIndex) {
        if(ui->suppliers->columnCount() != 1)
            ui->suppliers->removeColumn(logicalIndex);
    });

    connect(ui->comsumers->horizontalHeader(), &QHeaderView::sectionDoubleClicked, [this] (int logicalIndex) {
        if(ui->suppliers->columnCount() != 1)
            ui->comsumers->removeColumn(logicalIndex);
    });

    ui->suppliers->insertColumn(0);
    ui->comsumers->insertColumn(0);

    DelegateDouble *delegate = new DelegateDouble(Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignVCenter, this);

    ui->costs->setItemDelegate(delegate);
    ui->suppliers->setItemDelegate(delegate);
    ui->comsumers->setItemDelegate(delegate);
}

void MainWindow::initComboboxes() {
    ui->initMethod->addItem(tr("Метод северо-западного угла"), 0);
    ui->initMethod->addItem(tr("Метод наименьшей стоимости"), 1);
    ui->calculationMethod->addItem(tr("Распределительный метод"), 0);
    ui->calculationMethod->addItem(tr("Метод потенциалов"), 1);
}

void initActiveButton() {

}

void MainWindow::createConsole() const {
    Console *console = new Console();
    connect(this, &MainWindow::sendText, console, &Console::printText);
    connect(this, &MainWindow::sendHtml, console, &Console::printHtml);
    connect(this, &MainWindow::windowClosed, console, &Console::close);
    console->show();
}

void MainWindow::printText(QString text) const {
    if(receivers(SIGNAL(printText)) == 0) {
        createConsole();
    }
    emit sendText(text);
}

void MainWindow::printHtml(QString html)  const {
    if(receivers(SIGNAL(printHtml)) == 0) {
        createConsole();
    }
    emit sendHtml(html);
}

bool MainWindow::checkCosts() const {

}

bool MainWindow::checkSuplliers() const {

}

bool MainWindow::checkComsumers() const {

}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    emit windowClosed();
}
