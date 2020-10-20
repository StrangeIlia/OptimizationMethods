#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<MatrixPtr>();

    initTables();
    initComboboxesAndActiveButton();
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

void MainWindow::initTestVariant() {
    QVector<double> sullpierValues = {18, 19, 22, 19};
    ui->suppliers->setColumnCount(sullpierValues.size());
    for(int i = 0; i != sullpierValues.size(); ++i) {
        ui->suppliers->setItem(0, i, new QTableWidgetItem());
        ui->suppliers->item(0, i)->setData(Qt::DisplayRole, sullpierValues[i]);
    }

    QVector<double> comsumersValues = {14, 11, 17, 15, 14};
    ui->comsumers->setColumnCount(comsumersValues.size());
    for(int i = 0; i != comsumersValues.size(); ++i) {
        ui->comsumers->setItem(0, i, new QTableWidgetItem());
        ui->comsumers->item(0, i)->setData(Qt::DisplayRole, comsumersValues[i]);
    }

     QVector<QVector<double>> costValues = {
         {9,    21,     22,     14,     10},
         {30,   34,     42,     23,     26},
         {8,    17,     30,     27,     9},
         {11,   20,     24,     7,      25}
     };

     for(int i = 0; i != costValues.size(); ++i) {
         const auto &row = costValues[i];
         for(int j = 0; j != row.size(); ++j) {
            ui->costs->setItem(i, j, new QTableWidgetItem());
            ui->costs->item(i, j)->setData(Qt::DisplayRole, costValues[i][j]);
         }
     }
}

void MainWindow::calculate() {
    MatrixPtr costs = matrix(ui->costs);
    MatrixPtr suppliers = matrix(ui->suppliers);
    MatrixPtr comsumers = matrix(ui->comsumers);
    if(costs == nullptr) {
        QMessageBox message(this);
        message.setWindowTitle(tr("Ошибка ввода"));
        message.setText(tr("Неверно введены стоимости перевозок"));
        message.exec();
        return;
    }
    if(suppliers == nullptr) {
        QMessageBox message(this);
        message.setWindowTitle(tr("Ошибка ввода"));
        message.setText(tr("Неверно поставщики"));
        message.exec();
        return;
    }
    if(comsumers == nullptr) {
        QMessageBox message(this);
        message.setWindowTitle(tr("Ошибка ввода"));
        message.setText(tr("Неверно получатели"));
        message.exec();
        return;
    }

    emit sendData(suppliers, comsumers, costs);
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

void MainWindow::initComboboxesAndActiveButton() {
    QThread *thread = new QThread();
    Calculator *calculator = new Calculator();
    calculator->moveToThread(thread);

    connect(ui->initTestVariant, &QPushButton::clicked, [this] (bool /*ignored*/){
        this->initTestVariant();
    });

    for(auto item : calculator->initMethods()) {
        ui->initMethod->addItem(item);
    }

    for(auto item : calculator->calculateMethods()) {
        ui->calculationMethod->addItem(item);
    }

    connect(this, &MainWindow::sendData, calculator, &Calculator::calculate);
    connect(calculator->buffer(), &TextBuffer::flushed, this, &MainWindow::printHtml);
    connect(ui->initMethod, SIGNAL(currentIndexChanged(const QString&)), calculator, SLOT(setInitMethod(const QString&)));
    connect(ui->calculationMethod, SIGNAL(currentIndexChanged(const QString&)), calculator, SLOT(setCalcMethod(const QString&)));
    connect(ui->findPlan, &QPushButton::clicked, [this](bool /*ignored*/) {
       this->calculate();
    });

    thread->start();
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

MatrixPtr MainWindow::matrix(QTableWidget *widget) {
    MatrixPtr matrix(new Matrix(widget->rowCount(), widget->columnCount()));
    for(int i = 0; i != matrix->rows(); ++i) {
        for(int j = 0; j != matrix->columns(); ++j) {
            auto item = widget->item(i, j);
            if(item == nullptr) return nullptr;
            auto variantValue = item->data(Qt::DisplayRole);
            if(!variantValue.canConvert<double>()) return nullptr;
            double value = variantValue.value<double>();
            if(value <= 0) return nullptr;
            matrix->setCell(i, j, value);
        }
    }
    return matrix;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    emit windowClosed();
}
