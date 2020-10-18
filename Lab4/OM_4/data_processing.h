#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "main_data_struct.h"
#include "math/matrix/MatrixOnRow.hpp"

typedef MatrixOnRow<double>     Matrix;
typedef QSharedPointer<Matrix>  MatrixPtr;

class DataProcessing
{
public:
    DataProcessing();
    virtual ~DataProcessing();

    virtual void processing(MainDataStructPtr data) = 0;
};

#endif // DATAPROCESSING_H
