#ifndef LEASTCOSTMETHOD_H
#define LEASTCOSTMETHOD_H

#include <algorithm>

#include "data_processing.h"

class LeastCostMethod : public DataProcessing
{
public:
    LeastCostMethod(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs);

    void init(MainDataStructPtr data) override;
    bool oneStep(MainDataStructPtr data) override;

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;
    MatrixPtr _costs;

    Matrix suppliers;
    Matrix comsumers;
};

#endif // LEASTCOSTMETHOD_H
