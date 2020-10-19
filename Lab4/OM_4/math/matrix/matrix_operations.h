#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H

#include <limits>

#include "MatrixOnRow.hpp"

class MatrixOperations
{
public:
    MatrixOperations();

    template<class Base, class Index = int>
    static void swapRows(MatrixOnRow<Base, Index> &matrix, int firstRow, int secondRow);

    template<class Base, class Index = int>
    static bool slay(MatrixOnRow<Base, Index> &left, MatrixOnRow<Base, Index> &right);
};

#endif // MATRIXOPERATIONS_H
