#include "nortwest_corner_method.h"

NortwestCornerMethod::NortwestCornerMethod(MatrixPtr suppliers, MatrixPtr comsumers) {
    _suppliers = suppliers;
    _comsumers = comsumers;
}

void NortwestCornerMethod::init(MainDataStructPtr data) {
    data->clear();

    _currentSuppliers = *_suppliers;
    _currentComsumers = *_comsumers;

    _currentSuppliers.setSize(1, _currentSuppliers.rows() * _currentSuppliers.columns());
    _currentComsumers.setSize(1, _currentComsumers.rows() * _currentComsumers.columns());

    i = 0; j = 0;
}

bool NortwestCornerMethod::oneStep(MainDataStructPtr data) {
    if(i == _currentSuppliers.columns() || j == _currentComsumers.columns())
        return false;

    if(_currentSuppliers(0, i) < _currentComsumers(0, j)) {
        _currentComsumers(0, j) -= _currentSuppliers(0, i);
        data->insertIndex(i, j, _currentSuppliers(0, i));
        _currentSuppliers(0, i) = 0;
        ++i;
    } else {
        _currentSuppliers(0, i) -= _currentComsumers(0, j);
        data->insertIndex(i, j, _currentComsumers(0, j));
        _currentComsumers(0, j) = 0;
        ++j;
    }

    return true;
}
