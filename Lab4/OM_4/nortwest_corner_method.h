#ifndef NORTWESTCORNERMETHOD_H
#define NORTWESTCORNERMETHOD_H

#include "data_processing.h"

class NortwestCornerMethod : public DataProcessing
{
public:
    NortwestCornerMethod(MatrixPtr suppliers, MatrixPtr comsumers);

    void processing(MainDataStructPtr data) override;

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;
};

#endif // NORTWESTCORNERMETHOD_H
