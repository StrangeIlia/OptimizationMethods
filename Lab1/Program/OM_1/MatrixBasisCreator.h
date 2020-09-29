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
    MatrixBasisCreator(MatrixOnRow<Base, Index> *trackedMatrix);
    /// Создания базиса в доступных строках (где нет базиса)
    /// Возвращает ложь, если создание базиса не удалось
    bool createBasis(Index column);

    /// Создания базиса в строке
    /// Возвращает ложь, если создание базиса не удалось
    bool createBasis(Index row, Index column);

    /// Удаляет базис этой строки из записи (матрица при этом не меняется)
    void deleteBasis(Index row);

    /// Возвращет ссылку на обрабатываемую матрицу
    MatrixOnRow<Base, Index> *trackedMatrix() const;

    /// Возращает номер столбца, являющийся базисом в данной строке
    /// Если такового нет, то возвращает -1 (int)
    Index getBasis(Index row) const;

    /// Возвращает все известные базисы
    QHash<Index, Index> getAllBasis() const;
};

#endif // MATRIXBASIS_H
