#ifndef POTENTIALMETHOD_H
#define POTENTIALMETHOD_H

#include "distribution_method.h"
#include "math/matrix/matrix_operations.h"

class PotentialMethod : public DistributionMethod
{
public:
    PotentialMethod(MatrixPtr costs);

    bool oneStep(MainDataStructPtr data) override;

private:
    QPair<QVector<double>, QVector<double>> calculatePotential(MainDataStructPtr data);
};

#endif // POTENTIALMETHOD_H
