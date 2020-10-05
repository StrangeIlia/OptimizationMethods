#ifndef MATRIXBASIS_H
#define MATRIXBASIS_H

#include <QHash>
#include <limits>
#include <stdexcept>

#include "math/matrix/MatrixOnRow.hpp"

template<class Base, class Index = int>
class MatrixBasisCreator
{
    /// Ключ - номер строки, значение - базис (номер столбца)
    QHash<Index, Index> basis;
    MatrixOnRow<Base, Index> *matrix;
public:
    MatrixBasisCreator(MatrixOnRow<Base, Index> *trackedMatrix) {
        matrix = trackedMatrix;
    }

    /// Создания базиса в доступных строках (где нет базиса)
    /// Возвращает ложь, если создание базиса не удалось
    bool createBasis(Index column) {
        if(column < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть меньше 0");
        if(column > matrix->columns()) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть больше размеров матрицы");
        if(basis.values().contains(column)) return true;
        Index selectedRow = -1;
        for(Index i = 0; i != matrix->rows(); ++i) {
            if(basis.contains(i)) continue;
            if(std::abs(matrix->cell(i, column)) > std::numeric_limits<Base>::epsilon()) {
                selectedRow = i;
                break;
            }
        }
        return createBasis(selectedRow, column);
    }

    /// Создания базиса в строке
    /// Возвращает ложь, если создание базиса не удалось
    bool createBasis(Index row, Index column) {
        if(row < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: row не может быть меньше 0");
        if(column < 0) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть меньше 0");
        if(row >= matrix->rows()) throw std::invalid_argument("MatrixBasisCreator::createBasis: row не может быть больше размеров матрицы");
        if(column > matrix->columns()) throw std::invalid_argument("MatrixBasisCreator::createBasis: column не может быть больше размеров матрицы");
        if(basis.find(row) != basis.end()) {
            if(basis.find(row).value() == column)
                return true;
        }
        Base value = matrix->cell(row, column);
        if(std::abs(value) < std::numeric_limits<Base>::epsilon())
            return false;

        basis.remove(row);

        value = 1 / value;
        for(Index j = 0; j != matrix->columns(); ++j)
            matrix->setCell(row, j, matrix->cell(row, j) * value);

        for(Index i = 0; i != matrix->rows(); ++i) {
            if(i == row) continue;
            value = matrix->cell(i, column);
            for(int j = 0; j != matrix->columns(); ++j)
                matrix->setCell(i, j, matrix->cell(i, j) - matrix->cell(row, j) * value);
        }

        basis.insert(row, column);

        return true;
    }

    /// Удаляет базис этой строки из записи (матрица при этом не меняется)
    void deleteBasis(Index row) {
        basis.remove(row);
    }


    /// Возвращет ссылку на обрабатываемую матрицу
    MatrixOnRow<Base, Index> *trackedMatrix() const {
        return matrix;
    }

    /// Возращает номер столбца, являющийся базисом в данной строке
    /// Если такового нет, то возвращает -1 (int)
    Index getBasis(Index row) const {
        auto result = basis.find(row);
        if(result == basis.end())
            return  -1;
        else
            return result.value();
    }

    /// Возвращает все известные базисы
    const QHash<Index, Index> &getAllBasis() const {
        return basis;
    }
};

#endif // MATRIXBASIS_H
