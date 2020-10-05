#ifndef LARGEPENALTYITERATOR_H
#define LARGEPENALTYITERATOR_H

#include "SimplexMethodIterator.hpp"

template<class Base, class Index = int>
class LargePenaltyIterator : public SimpleMethodIterator<Base, Index> {
protected:
    Index _countAdditionBasis;

public:
    /// Итератор считает:
    /// 1) Последняя строка - это целевая функция
    /// 2) Последний столбец - свободный член
    LargePenaltyIterator(MatrixOnRow<Base, Index> *matrix) : SimpleMethodIterator<Base, Index>(matrix) {

    }

    Index countAdditionBasis() const {
        return _countAdditionBasis;
    }

    bool init() override {
        if(!this->_creator->getAllBasis().empty())
            return true;
        normalizeFreeMembers();
        if(!initBasis()) {
            addAuxiliaryColumns();
            prepareFunction();
            initBasis();
        }
        return true;
    }

protected:
    Index firstAuxiliaryColumns() const {
        return this->_creator->trackedMatrix()->columns() - 1 - _countAdditionBasis;
    }

    /// Делает все свободные коэффициенты > 0
    void normalizeFreeMembers() {
        auto *matrix = this->_creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index indexFreeMember = matrix->columns() - 1;
        for(Index i = 0; i != systemRows; ++i) {
            /// Упорядочиваем
            if(matrix->cell(i, indexFreeMember) < 0) {
                for(Index j = 0; j != matrix->columns(); ++j) {
                    matrix->cell(i, j) = -matrix->cell(i, j);
                }
            }
        }
    }

    bool initBasis() {
        bool fullInit = true;
        auto *matrix = this->_creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(Index i = 0; i != systemRows; ++i) {
            Index result = findBasis(i);
            if(result != -1)
                this->_creator->createBasis(i, result);
            else {
                fullInit = false;
            }
        }
        return fullInit;
    }

    void addAuxiliaryColumns() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index basisCount = creator->getAllBasis().count();
        if(basisCount == systemRows) return;
        Index indexFreeMember = matrix->columns() - 1;
        /// Добавляем вспомогательные переменные
        for(Index i = 0; i != systemRows; ++i) {
            if(creator->getBasis(i) == -1) {
                matrix->insertColumn(indexFreeMember, 1);
                matrix->cell(i, indexFreeMember) = 1;
                ++indexFreeMember;
                ++_countAdditionBasis;
            }
        }
    }

    virtual void prepareFunction() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;

        /// Ищем максимальный по модулю коэффициент
        Base maxValue = -1;
        for(Index i = 0; i != matrix->rows(); ++i) {
            for(Index j = 0; j != matrix->columns(); ++j) {
                auto value = std::abs(matrix->cell(i, j));
                maxValue = std::max(value, maxValue);
            }
        }

        maxValue *= 10;
        /// Добавляем инициализируем новую функцию
        Index start = firstAuxiliaryColumns();
        Index end = start + _countAdditionBasis;
        for(Index i = start; i != end; ++i) {
            matrix->cell(systemRows, i) = 1;
        }
    }

    void removeAdditionVariable() {
        auto *matrix = this->_creator->trackedMatrix();
        /// Удаляем вспомогательные столбцы
        Index first = firstAuxiliaryColumns();
        Index last = first + _countAdditionBasis - 1;
        matrix->removeColumns(first, last);
        _countAdditionBasis = 0;
    }

    virtual void returnFunction() {

    }

    bool oneStep() override {
        bool result = this->SimpleMethodIterator<Base, Index>::oneStep();
        if(!result) {
            if(_countAdditionBasis == 0)
                return false;
            removeAdditionVariable();
            returnFunction();
            return true;
        }
        return result;
    }
};

#endif // LARGEPENALTYITERATOR_H
