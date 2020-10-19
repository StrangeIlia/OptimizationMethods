#ifndef LEASTCOSTMETHOD_H
#define LEASTCOSTMETHOD_H

#include <algorithm>

#include "data_handler.h"

class LeastCostMethod : public DataHandler
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
