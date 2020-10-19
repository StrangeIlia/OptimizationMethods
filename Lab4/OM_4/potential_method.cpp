#include "potential_method.h"

PotentialMethod::PotentialMethod(MatrixPtr costs) : DistributionMethod(costs) {

}

bool PotentialMethod::oneStep(MainDataStructPtr data) {
    auto potentials = calculatePotential(data);
    for(int i = 0; i != _costs->rows(); ++i) {
        for(int j = 0; j != _costs->columns(); ++j) {
            if(data->hasIndex(i, j)) continue;
            double value = potentials.first[i] + potentials.second[j];
            if(value < 0) {
                auto cycle = createCycle(i, j, data);
                auto removedIndex = updateCycle(cycle);
                auto additionIndex = cycle.front();
                data->removeIndex(removedIndex);
                data->insertIndex(additionIndex);
                return true;
            }
        }
    }
    return false;
}

QPair<QVector<double>, QVector<double>> PotentialMethod::calculatePotential(MainDataStructPtr data) {
    int countVariable = _costs->rows() + _costs->columns() - 1;
    Matrix left(countVariable, countVariable + 1), right(countVariable, 1);

    int row = 0;
    for(int i = 0; i != left.rows(); ++i) {
        auto columns = data->columns(i);
        for(auto index : columns) {
            left(row, index->row) = 1;
            left(row, index->column + _costs->rows()) = 1;
            right(row, 0) = _costs->cell(index->row, index->column);
            ++row;
        }
    }

    MatrixOperations::slay(left, right);
    QVector<double> suppliers; suppliers.reserve(_costs->rows());
    QVector<double> comsumers; comsumers.reserve(_costs->columns());
    for(int i = 0; i != _costs->rows(); ++i) {
        suppliers.push_back(right(i, 0));
    }
    for(int i = 0; i != _costs->columns(); ++i) {
        comsumers.push_back(right(i + _costs->rows(), 0));
    }

    return QPair<QVector<double>, QVector<double>>(suppliers, comsumers);
}
