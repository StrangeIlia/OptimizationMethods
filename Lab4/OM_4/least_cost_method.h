#ifndef LEASTCOSTMETHOD_H
#define LEASTCOSTMETHOD_H

#include <algorithm>

#include "data_processing.h"

class LeastCostMethod : public DataProcessing
{
public:
    LeastCostMethod(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs);

    void processing(MainDataStructPtr data) override;

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;
    MatrixPtr _costs;
};

#endif // LEASTCOSTMETHOD_H
