#include "BasisIterator.h"

BasisIterator::BasisIterator(MatrixOnRow<double> *matrix) : _creator(new MatrixBasisCreator<double>(matrix))
{
    removeUnusedVariableAndRows();
    if(matrix->rows() >= matrix->columns())
        throw std::invalid_argument("BasisIterator::BasisIterator: число строк должно быть меньше или равно числу столбцоа");
}

BasisIterator::~BasisIterator() {
    delete _creator;
}

bool BasisIterator::next() {
    if(_basis.size() == 0)
        return createFirstBasis();
    auto matrix = _creator->trackedMatrix();
    while(!increment()) {
        if(_basis.empty()) return false;
    }
    while(_basis.size() != matrix->rows()) {
        _basis.push(_basis.top());
        while(!increment()) {
            if(_basis.empty()) return false;
        }
    }
    return true;
}


QSet<int> BasisIterator::removedVariables() const {
    return _removedVariables;
}

MatrixBasisCreator<double> *BasisIterator::creator() const {
    return _creator;
}


bool BasisIterator::increment() {
    ++_basis.top();
    auto matrix = _creator->trackedMatrix();
    int max = matrix->columns() - matrix->rows() + _basis.size() - 1;
    while(_basis.top() != max && !_creator->createBasis(_basis.size() - 1, _basis.top()))
        ++_basis.top();
    if(_basis.top() == max) {
        _basis.pop();
        return false;
    }
    return true;
}

bool BasisIterator::createFirstBasis() {
    _basis.push(-1);
    auto matrix = _creator->trackedMatrix();
    goto start_basis_create;
    while(_basis.size() != matrix->rows()) {
        _basis.push(_basis.top());
        start_basis_create:
        if(!increment()) errorCorrention();
    }
    return !_basis.empty();
}

void BasisIterator::errorCorrention() {
    auto matrix = _creator->trackedMatrix();
    int rowIndex = _basis.size() - 1;
    bool isEmptyRow = true;
    int indexFreeMember = matrix->columns() - 1;
    for(int j = 0; j != indexFreeMember; ++j) {
        if(std::abs(matrix->cell(rowIndex, j)) > std::numeric_limits<double>::epsilon()) {
            isEmptyRow = false;
            break;
        }
    }
    if(isEmptyRow) {
        if(std::abs(matrix->cell(rowIndex, indexFreeMember)) > std::numeric_limits<double>::epsilon())
            throw std::exception();
        matrix->removeRows(rowIndex, rowIndex);
    }
    std::exception();
}

void BasisIterator::removeUnusedVariableAndRows() {
    auto matrix = _creator->trackedMatrix();
    /// Чистка переменных
    for(int j = 0; j !=  matrix->columns() - 1; ) {
        bool isUnusedVariable = true;
        for(int i = 0; i != matrix->rows(); ++i) {
            if(std::abs(matrix->cell(i, j)) > std::numeric_limits<double>::epsilon()) {
                isUnusedVariable = false;
                break;
            }
        }
        if(isUnusedVariable) {
            _removedVariables.insert(j);
            matrix->removeColumns(j, j);
        }
        else ++j;
    }
    /// Чистка строк
    for(int i = 0; i != matrix->rows();) {
        bool isUnusedRow = true;
        for(int j = 0; j !=  matrix->columns(); ++j) {
            auto value = std::abs(matrix->cell(i, j));
            if(value > std::numeric_limits<double>::epsilon()) {
                isUnusedRow = false;
                break;
            }
        }
        if(isUnusedRow)
            matrix->removeRows(i, i);
        else ++i;
    }
}
