#ifndef DISTRIBUTIONMETHOD_H
#define DISTRIBUTIONMETHOD_H

#include <QStack>

#include "data_handler.h"

class DistributionMethod : public DataHandler
{
public:
    DistributionMethod(MatrixPtr costs);

    bool oneStep(MainDataStructPtr data) override;

protected:
    MatrixPtr _costs;

    static QList<VariantIndexPtr> createCycle(int row, int column, MainDataStructPtr data);
    double cycleCost(QList<VariantIndexPtr> cycle) const;
    static VariantIndexPtr updateCycle(QList<VariantIndexPtr> cycle);
};

#endif // DISTRIBUTIONMETHOD_H
