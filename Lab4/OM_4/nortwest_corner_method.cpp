#include "nortwest_corner_method.h"

NortwestCornerMethod::NortwestCornerMethod(MatrixPtr suppliers, MatrixPtr comsumers) {
    _suppliers = suppliers;
    _comsumers = comsumers;
}

void NortwestCornerMethod::processing(MainDataStructPtr data) {
    data->clear();

    Matrix suppliers = *_suppliers;
    Matrix comsumers = *_comsumers;

    suppliers.setSize(1, suppliers.rows() * suppliers.columns());
    comsumers.setSize(1, comsumers.rows() * comsumers.columns());

    int i = 0; int j = 0;

    while(i != suppliers.rows() && j != comsumers.columns()) {
        data->insertIndex(0, 0);
        if(suppliers(0, i) < comsumers(0, j)) {
            comsumers(0, j) -= suppliers(0, i);
            suppliers(0, i) = 0;
            ++i;
        } else {
            suppliers(0, i) -= comsumers(0, j);
            comsumers(0, j) = 0;
            ++j;
        }
    }
}
