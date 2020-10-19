#ifndef NORTWESTCORNERMETHOD_H
#define NORTWESTCORNERMETHOD_H

#include "data_handler.h"

class NortwestCornerMethod : public DataHandler
{
public:
    NortwestCornerMethod(MatrixPtr _currentSuppliers, MatrixPtr _currentComsumers);

    void init(MainDataStructPtr data) override;
    bool oneStep(MainDataStructPtr data) override;

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;

    Matrix _currentSuppliers;
    Matrix _currentComsumers;

    int i, j;
};

#endif // NORTWESTCORNERMETHOD_H
