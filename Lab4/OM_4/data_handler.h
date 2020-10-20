#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QDebug>

#include "main_data_struct.h"
#include "math/matrix/MatrixOnRow.hpp"

class DataHandler;

typedef MatrixOnRow<double>             Matrix;
typedef QSharedPointer<Matrix>          MatrixPtr;
typedef QSharedPointer<DataHandler>     DataHandlerPtr;

class DataHandler
{
public:
    DataHandler();
    virtual ~DataHandler();

    virtual void init(MainDataStructPtr data);
    virtual bool oneStep(MainDataStructPtr data) = 0;
    virtual void processing(MainDataStructPtr data);
};



#endif // DATAPROCESSING_H
