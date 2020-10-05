#ifndef LARGEPENALTYITERATOR_H
#define LARGEPENALTYITERATOR_H

#include "SimplexMethodIterator.hpp"

template<class Base, class Index = int>
class LargePenaltyIterator : public SimpleMethodIterator<Base, Index> {
protected:
    Index _countAdditionBasis = 0;

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
                    matrix->setCell(i, j, -matrix->cell(i, j));
                }
            }
        }
    }

    bool initBasis() {
        bool fullInit = true;
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        for(Index i = 0; i != systemRows; ++i) {
            if(creator->getBasis(i) != -1) continue;
            Index result = this->findBasis(i);
            if(result != -1)
                creator->createBasis(i, result);
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
                matrix->insertColumns(indexFreeMember, 1);
                matrix->setCell(i, indexFreeMember, 1);
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
            matrix->setCell(systemRows, i, maxValue);
        }
    }

    bool reqRemoveAdditionVariable() {
        if(_countAdditionBasis == 0) return false;
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index freeMember = matrix->columns() - 1;
        Index first = firstAuxiliaryColumns();
        Index last = first + _countAdditionBasis;
        for(Index i = 0; i != systemRows; ++i) {
            if(creator->getBasis(i) >= first)
                return false;
        }
        return true;
    }

    bool removeAdditionBasis() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index freeMember = matrix->columns() - 1;
        Index first = firstAuxiliaryColumns();
        Index last = first + _countAdditionBasis;
        for(Index i = first; i != last; ++i) {
            Index oldBasis = creator->getBasis(i);
            if(oldBasis >= first) {
                if(std::abs(matrix->cell(i, freeMember)) > std::numeric_limits<Base>::epsilon()) {
                    return false;
                }
                creator->deleteBasis(i);
                bool okey = false;
                for(Index j = 0; !okey && j != first; ++j) {
                    okey = creator->createBasis(j);
                }
                if(!okey) {
                    okey = creator->createBasis(i, oldBasis);
                    return false;
                }
            }
        }
        return true;
    }

    void removeAdditionVariable() {
        auto *creator = this->_creator;
        auto *matrix = creator->trackedMatrix();
        Index systemRows = matrix->rows() - 1;
        Index first = firstAuxiliaryColumns();
        Index last = first + _countAdditionBasis - 1;
        /// Удаляем вспомогательные столбцы
        matrix->removeColumns(first, last);
        _countAdditionBasis = 0;
    }

    virtual void returnFunction() {

    }

    bool oneStep() override {
        if(reqRemoveAdditionVariable()) {
            removeAdditionVariable();
            returnFunction();
            return true;
        }
        bool result = this->SimpleMethodIterator<Base, Index>::oneStep();
        if(!result) {
            if(_countAdditionBasis == 0)
                return false;
            if(!removeAdditionBasis())
                return false;
            removeAdditionVariable();
            returnFunction();
            return true;
        }
        return result;
    }
};

#endif // LARGEPENALTYITERATOR_H
