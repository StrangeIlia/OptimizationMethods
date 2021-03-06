#include "least_cost_method.h"

LeastCostMethod::LeastCostMethod(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs) {
    _suppliers = suppliers;
    _comsumers = comsumers;
    _costs = costs;
}

void LeastCostMethod::init(MainDataStructPtr data) {
    data->clear();

    suppliers = *_suppliers;
    comsumers = *_comsumers;

    suppliers.setSize(1, suppliers.rows() * suppliers.columns());
    comsumers.setSize(1, comsumers.rows() * comsumers.columns());
}

bool LeastCostMethod::oneStep(MainDataStructPtr data) {
    double eps = std::numeric_limits<double>::epsilon();
    double minValue = std::numeric_limits<double>::max();
    int minI = -1, minJ;
    for(int i = 0; i != suppliers.columns(); ++i) {
        if(suppliers(0, i) < eps) continue;
        for(int j = 0; j != comsumers.columns(); ++j) {
            if(comsumers(0, j) < eps) continue;
            if(_costs->cell(i, j) < minValue) {
                minValue = _costs->cell(i, j);
                minI = i;
                minJ = j;
            }
        }
    }
    if(minI == -1) return false;

    minValue = std::min(suppliers(0, minI), comsumers(0, minJ));
    suppliers(0, minI) -= minValue;
    comsumers(0, minJ) -= minValue;
    data->insertIndex(minI, minJ, minValue);

    return true;
}
