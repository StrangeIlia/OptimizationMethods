#ifndef TRANSPORTTASKLOGGER_H
#define TRANSPORTTASKLOGGER_H

#include "html_logger.h"
#include "data_handler.h"

class TransportTaskLogger;

typedef QSharedPointer<TransportTaskLogger>     TransportTaskLoggerPtr;

class TransportTaskLogger : public HtmlLogger
{
public:
    TransportTaskLogger(QTextStreamPtr stream);

    bool isValid() const;

    MatrixPtr costs() const;
    MatrixPtr suplliers() const;
    MatrixPtr comsumers() const;

    void setCosts(MatrixPtr ptr);
    void setSuppliers(MatrixPtr ptr);
    void setComsumers(MatrixPtr ptr);

    void printTransportTable(MainDataStructPtr data);

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;
    MatrixPtr _costs;
};

#endif // TRANSPORTTASKLOGGER_H
