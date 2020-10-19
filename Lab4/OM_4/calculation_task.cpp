#include "calculation_task.h"

CalculationTask::CalculationTask(DataHandlerPtr handler, MainDataStructPtr basis,
                                 TransportTaskLoggerPtr logger, QObject *parent) : QObject(parent) {
    _basis = basis;
    _logger = logger;
    _handler = handler;
}

void CalculationTask::run() {
    _handler->init(_basis);

    if(_logger == nullptr) {
        _handler->processing(_basis);
        emit finished();
        return;
    }

    auto &out = _logger->out();
    int stepNumber = 1;
    while(_handler->oneStep(_basis)) {
        auto listBasis = _basis->data();
        out << tr("Шаг номер №") << stepNumber << "<br>";
        _logger->printTransportTable(_basis);
        out << "<br><br>";
        ++stepNumber;
    }

    emit finished();
}

DataHandlerPtr CalculationTask::handler() {
    return _handler;
}

MainDataStructPtr CalculationTask::basis() {
    return _basis;
}

TransportTaskLoggerPtr CalculationTask::logger() const {
    return _logger;
}
