#ifndef SIMPLEXMETHODITERATOR_H
#define SIMPLEXMETHODITERATOR_H

#include <limits>
#include <algorithm>
#include "MatrixBasisCreator.hpp"

template<class Base, class Index = int>
class SimpleMethodIterator {
protected:
    MatrixBasisCreator<Base, Index> *_creator;
public:
    /// Итератор считает:
    /// 1) Последняя строка - это целевая функция
    /// 2) Последний столбец - свободный член
    SimpleMethodIterator(MatrixOnRow<Base, Index> *matrix) {
        _creator = new MatrixBasisCreator<Base, Index>(matrix);
    }

    virtual bool init() {
        if(!_creator->getAllBasis().empty())
            return true;
        if(!checkFreeMembers())
            return false;
        return initBasis();
    }

    virtual bool next() {
        if(_creator->getAllBasis().empty() && !initBasis())
            return false;
        return oneStep();
    }

    inline MatrixBasisCreator<Base, Index> *creator() const {
        return _creator;
    }

protected:
    bool oneStep() {
        MatrixOnRow<Base, Index> *matrix = _creator->trackedMatrix();

        Index functionRow = matrix->rows() - 1;
        std::vector<Index> columnCandidats;
        columnCandidats.reserve(matrix->columns());
        for(Index j = 0; j != matrix->columns(); ++j) {
            if(matrix->cell(functionRow, j) < 0)
                columnCandidats.push_back(j);
        }

        if(columnCandidats.empty())
            return false;

        std::sort(columnCandidats.begin(), columnCandidats.end(), [matrix, functionRow] (const Index &first, const Index &second) {
            return matrix->cell(functionRow, first) < matrix->cell(functionRow, second);
        });

        Index bestColumn;
        Index freeMemberIndex = matrix->columns() - 1;
        std::vector<Index> rowCandidats;
        rowCandidats.reserve(matrix->rows());
        for(Index column : columnCandidats) {
            for(Index i = 0; i != functionRow; ++i) {
                auto cellValue = matrix->cell(i, column);
                auto freeMember = matrix->cell(i, freeMemberIndex);
                if(cellValue > 0 && freeMember > 0) {
                    rowCandidats.push_back(i);
                }
            }
            if(!rowCandidats.empty()) {
                bestColumn = column;
                break;
            }
        }

        if(rowCandidats.empty()) return false;

        Index bestRow;

        Base bestValue = std::numeric_limits<Base>::max();
        for(const Index &row : rowCandidats) {
            Base value = matrix->cell(row, freeMemberIndex) / matrix->cell(row, bestColumn);
            if(value < bestValue) {
                bestValue = value;
                bestRow = row;
            }
        }

        _creator->createBasis(bestRow, bestColumn);
        return true;
    }

    /// Проверка свободных коэффициентов
    bool checkFreeMembers() {
        auto *matrix = _creator->trackedMatrix();
        Index indexFreeMember = matrix->columns() - 1;
        for(Index i = 0; i != matrix->rows(); ++i) {
            if(matrix->cell(i, indexFreeMember) < 0)
                return false;
        }
        return true;
    }

    /// Инициализация метода
    bool initBasis() {
        auto *matrix = _creator->trackedMatrix();
        /// Проверка свободных коэффициентов
        Index systemRows = matrix->rows() - 1;
        for(Index i = 0; i != systemRows; ++i) {
            Index result = findBasis(i);
            if(result == -1) {
                clearAllBasis();
                return false;
            }
            _creator->createBasis(i, result);
        }
        return true;
    }

    void clearAllBasis() {
        auto *matrix = _creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(Index i = 0; i != systemRows; ++i)
            _creator->deleteBasis(i);
    }

    /// Находит базисный столбец
    Index findBasis(Index row, Index firstColumn = 0, Index lastColumn = -1) {
        auto *matrix = _creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        if(lastColumn == -1) lastColumn = matrix->columns() - 1;
        auto eps = std::numeric_limits<Base>::epsilon();
        for(Index j = firstColumn; j < lastColumn; ++j) {
            if(std::abs(matrix->cell(row, j) - 1) < eps) {
                bool find = true;
                for(Index i = 0; i < systemRows; ++i) {
                    if(i == row) continue;
                    if(std::abs(matrix->cell(i, j)) > eps) {
                        find = false;
                        break;
                    }
                }
                if(find) return j;
            }
        }
        return -1;
    }
};

#endif // SIMPLEXMETHODITERATOR_H
