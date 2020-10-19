#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "main_data_struct.h"
#include "math/matrix/MatrixOnRow.hpp"

class DataProcessing;

typedef MatrixOnRow<double>             Matrix;
typedef QSharedPointer<Matrix>          MatrixPtr;
typedef QSharedPointer<DataProcessing>  DataProcessingPtr;

class DataProcessing
{
public:
    DataProcessing();
    virtual ~DataProcessing();

    virtual void init(MainDataStructPtr data);
    virtual bool oneStep(MainDataStructPtr data) = 0;
    virtual void processing(MainDataStructPtr data);
};



#endif // DATAPROCESSING_H
