#ifndef DISTRIBUTIONMETHOD_H
#define DISTRIBUTIONMETHOD_H

#include <QStack>

#include "data_processing.h"

class DistributionMethod : public DataProcessing
{
public:
    DistributionMethod(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs);

    void processing(MainDataStructPtr data) override;

private:
    MatrixPtr _suppliers;
    MatrixPtr _comsumers;
    MatrixPtr _costs;



    static QList<VariantIndexPtr> createCycle(int row, int column, MainDataStructPtr data);

    double cycleCost(QList<VariantIndexPtr> cycle) const;
};

#endif // DISTRIBUTIONMETHOD_H
