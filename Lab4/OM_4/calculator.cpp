#include "calculator.h"

QSet<QString> Calculator::_initMethods;
QSet<QString> Calculator::_calculateMethods;

QString nortwestCornerMethod;
QString leastCostMethod;

QString distributionMethod;
QString potentialMethod;

Calculator::Calculator(QObject *parent) : QObject(parent) {
    _buffer = new TextBuffer;
    if(_initMethods.size() == 0) {
        leastCostMethod = tr("Метод наименьшей стоимости");
        nortwestCornerMethod = tr("Метод северо-западного угла");

        _initMethods.insert(leastCostMethod);
        _initMethods.insert(nortwestCornerMethod);
    }
    if(_calculateMethods.size() == 0) {
        potentialMethod = tr("Метод потенциалов");
        distributionMethod = tr("Распределительный метод");

        _calculateMethods.insert(potentialMethod);
        _calculateMethods.insert(distributionMethod);
    }
    _init = leastCostMethod;
    _calculate = potentialMethod;
}

Calculator::~Calculator() {
   delete _buffer;
}

TextBuffer *Calculator::buffer() const {
    return _buffer;
}

QSet<QString> Calculator::initMethods() {
    return _initMethods;
}

QSet<QString> Calculator::calculateMethods() {
    return _calculateMethods;
}

void Calculator::calculate(MatrixPtr suplliers, MatrixPtr comsumers, MatrixPtr costs) {
    if(suplliers->rows() != 1)  {
        return;
    }
    if(comsumers->rows() != 1) {
        return;
    }
    if(suplliers->columns() != costs->rows()) {
        return;
    }
    if(comsumers->columns() != costs->columns()) {
        return;
    }


    DataHandlerPtr init, calc;

    if(_init == nortwestCornerMethod) {
        init.reset(new NortwestCornerMethod(suplliers, comsumers));
    } else if(_init == leastCostMethod) {
        init.reset(new LeastCostMethod(suplliers, comsumers, costs));
    } else {
        return;
    }

    if(_calculate == distributionMethod) {
        calc.reset(new DistributionMethod(costs));
    } else if(_calculate == potentialMethod) {
        calc.reset(new PotentialMethod(costs));
    } else {
        return;
    }


    MainDataStructPtr basis(new MainDataStruct);
    TransportTaskLoggerPtr logger(new TransportTaskLogger(_buffer->stream()));

    logger->setCosts(costs);
    logger->setSuppliers(suplliers);
    logger->setComsumers(comsumers);

    CalculationTask *initTask = new CalculationTask(init, basis, logger, this);
    CalculationTask *calcTask = new CalculationTask(calc, basis, logger, this);

    logger->message(tr("Начальные условия закрытой транспортной задачи"));
    logger->printTransportTable(basis);

    double numberOfGoods = 0;
    for(int i = 0; i != suplliers->columns(); ++i) {
        numberOfGoods += suplliers->cell(0, i);
    }
    for(int i = 0; i != comsumers->columns(); ++i) {
        numberOfGoods -= comsumers->cell(0, i);
    }
    if(std::abs(numberOfGoods) > std::numeric_limits<double>::epsilon()) {
        double maxValue = -1;
        for(int i = 0; i != costs->rows(); ++i) {
            for(int j = 0; j != costs->columns(); ++j) {
                maxValue = std::max(std::abs(costs->cell(i, j)), maxValue);
            }
        }
        maxValue *= 100;

        logger->message(tr("Задача не является закрытой, поэтому преобразуем ее в закрытую"));
        if(numberOfGoods < 0) {
            suplliers->insertColumns(suplliers->columns(), 1);
            suplliers->setCell(0, suplliers->columns() - 1, -numberOfGoods);
            int rowNumber = costs->rows();
            costs->insertRows(rowNumber, 1);
            for(int j = 0; j != costs->columns(); ++j) {
                costs->setCell(rowNumber, j, maxValue);
            }
        } else {
            comsumers->insertColumns(comsumers->columns(), 1);
            comsumers->setCell(0, comsumers->columns() - 1,  numberOfGoods);
            int columnNumber = costs->columns();
            costs->insertColumns(columnNumber, 1);
            for(int i = 0; i != costs->rows(); ++i) {
                costs->setCell(i, columnNumber, maxValue);
            }
        }
    }

    logger->message(tr("Процесс инициализации: (") + _init + ")");
    initTask->run();
    logger->message(tr("Процесс нахождения решения: (") + _calculate + ")");
    calcTask->run();

    double cost = 0;
    for(auto index : basis->data()) {
        cost += costs->cell(index->row, index->column) * index->count;
    }
    logger->message(tr("Суммартная стоимость решения: ") + QString::number(cost));

    _buffer->flush();
    emit finished();
}

bool Calculator::setInitMethod(const QString &str) {
    if(!_initMethods.contains(str)) return false;
    _init = str;
    return true;
}

bool Calculator::setCalcMethod(const QString &str) {
    if(!_calculateMethods.contains(str)) return false;
    _calculate = str;
    return true;
}
