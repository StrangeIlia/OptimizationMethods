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
        Index indexAuxiliaryVariable = matrix->columns() - 1 - this->_countAdditionBasis;
        _baseFunction.resize(indexAuxiliaryVariable);
        /// Копируем исходную функцию и зануляем коэффициенты при ней
        for(Index j = 0; j != indexAuxiliaryVariable; ++j) {
            _baseFunction[j] = matrix->cell(systemRows, j);
            matrix->cell(systemRows, j) = 0;
        }
        /// Добавляем инициализируем новую функцию
        Index start = this->firstAuxiliaryColumns();
        Index end = start + this->_countAdditionBasis;
        for(Index i = start; i != end; ++i) {
            matrix->cell(systemRows, i) = 1;
        }
    }

    void returnFunction() override {
        if(_baseFunction.empty()) return;
        auto *matrix = this->_creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(int j = 0; j != _baseFunction.size(); ++j) {
            matrix->cell(systemRows, j) = _baseFunction[j];
        }
    }
};

#endif // ADDITIONVARIABLESITERATOR_H
