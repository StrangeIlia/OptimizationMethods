#include "MatrixBasisCreator.h"

template<class Base, class Index>
MatrixBasisCreator<Base, Index>::MatrixBasisCreator(MatrixOnRow<Base, Index> *trackedMatrix) {
    matrix = trackedMatrix;
}

template<class Base, class Index>
bool MatrixBasisCreator<Base, Index>::createBasis(Index column) {
    if(column < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть меньше 0");
    if(column > matrix->getColumns()) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть больше размеров матрицы");
    if(basis.values().contains(column)) return true;
    Index selectedRow = -1;
    for(Index i = 0; i != matrix->getRows(); ++i) {
        if(basis.contains(i)) continue;
        if(std::abs(matrix->getCell(i, column)) > std::numeric_limits<Base>::epsilon()) {
            selectedRow = i;
            break;
        }
    }
    return createBasis(selectedRow, column);
}

template<class Base, class Index>
bool MatrixBasisCreator<Base, Index>::createBasis(Index row, Index column) {
    if(row < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: row не может быть меньше 0");
    if(column < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть меньше 0");
    if(row >= matrix->getRows()) throw std::invalid_argument("MatrixBasisCreator::createBasis: row не может быть больше размеров матрицы");
    if(column > matrix->getColumns()) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть больше размеров матрицы");
    if(basis.find(row) == column) return true;
    Base value = matrix->getCell(row, column);
    if(std::abs(value) < std::numeric_limits<Base>::epsilon())
        return false;

    basis.remove(row);

    value = 1 / value;
    for(Index j = 0; j != matrix->getColumns(); ++j)
        matrix->setCell(row, j, matrix->getCell(row, j) * value);

    for(Index i = 0; i != matrix->getRows(); ++i) {
        if(i == row) continue;
        value = matrix->getCell(i, column);
        for(int j = 0; j != matrix->getColumns(); ++j)
            matrix->setCell(i, j, matrix->getCell(i, j) - matrix->getCell(row, j) * value);
    }

    basis.insert(row, column);

    return true;
}

template<class Base, class Index>
void MatrixBasisCreator<Base, Index>::deleteBasis(Index row) {
    basis.remove(row);
}

template<class Base, class Index>
MatrixOnRow<Base, Index> *MatrixBasisCreator<Base, Index>::trackedMatrix() const {
    return matrix;
}

template<class Base, class Index>
Index MatrixBasisCreator<Base, Index>::getBasis(Index row) const {
    auto result = basis.find(row);
    if(result == basis.end())
        return  -1;
    else
        return result.value();
}

template<class Base, class Index>
QHash<Index, Index>MatrixBasisCreator<Base, Index>:: getAllBasis() const {
    return basis;
}
