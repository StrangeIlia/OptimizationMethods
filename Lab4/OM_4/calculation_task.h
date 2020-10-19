#ifndef CALCULATIONTASK_H
#define CALCULATIONTASK_H

#include <QFile>
#include <QRunnable>
#include <QTextStream>

#include "data_handler.h"
#include "transport_task_logger.h"

/// Выполняет вычисления
/// Если указан поток, то отправляет в него отчет о работе в формате html
class CalculationTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    CalculationTask(DataHandlerPtr handler, MainDataStructPtr basis,
                    TransportTaskLoggerPtr logger = nullptr, QObject *parent = nullptr);

    void run() override;

    DataHandlerPtr handler();
    MainDataStructPtr basis();
    TransportTaskLoggerPtr logger() const;

signals:
    void finished();

private:
    TransportTaskLoggerPtr  _logger;
    DataHandlerPtr          _handler;
    MainDataStructPtr       _basis;
};

#endif // CALCULATIONTASK_H
