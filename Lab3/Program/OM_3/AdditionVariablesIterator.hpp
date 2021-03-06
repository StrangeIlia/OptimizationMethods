#ifndef ADDITIONVARIABLESITERATOR_H
#define ADDITIONVARIABLESITERATOR_H

#include "LargePenaltyIterator.hpp"

template<class Base, class Index = int>
class AdditionVariablesIterator : public LargePenaltyIterator<Base, Index> {
protected:
    std::vector<Base> _baseFunction;

public:
    /// Итератор считает:
    /// 1) Последняя строка - это целевая функция
    /// 2) Последний столбец - свободный член
    AdditionVariablesIterator(MatrixOnRow<Base, Index> *matrix) : LargePenaltyIterator<Base, Index>(matrix) {

    }

protected:
    virtual void prepareFunction() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index indexAuxiliaryVariable = this->firstAuxiliaryColumns();
        _baseFunction.resize(indexAuxiliaryVariable);
        /// Копируем исходную функцию и зануляем коэффициенты при ней
        for(Index j = 0; j != indexAuxiliaryVariable; ++j) {
            _baseFunction[j] = matrix->cell(systemRows, j);
            matrix->setCell(systemRows, j, 0);
        }
        /// Добавляем инициализируем новую функцию
        Index start = indexAuxiliaryVariable;
        Index end = start + this->_countAdditionBasis;
        for(Index i = start; i != end; ++i) {
            matrix->setCell(systemRows, i, 1);
        }
    }

    void updateBasis() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(Index i = 0; i != systemRows; ++i) {
            Index column = creator->getBasis(i);
            creator->deleteBasis(i);
            creator->createBasis(i, column);
        }
    }


    void returnFunction() override {
        if(_baseFunction.empty()) return;
        auto *creator = this->_creator;
        auto *matrix = this->_creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(int j = 0; j != _baseFunction.size(); ++j) {
            matrix->setCell(systemRows, j, _baseFunction[j]);
        }

        updateBasis();
    }
};

#endif // ADDITIONVARIABLESITERATOR_H
