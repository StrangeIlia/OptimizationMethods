#ifndef BASISITERATOR_H
#define BASISITERATOR_H

#include <QSet>
#include <QStack>

#include "MatrixBasisCreator.hpp"

class BasisIterator
{
public:
    /// При инициализации удаляет пустые строки и столбцы
    BasisIterator(MatrixOnRow<double>* matrix);
    ~BasisIterator();

    /// Возвращает истина, если удалось найти новый базис
    /// Может удалять, образовавшиеся в процессе работы, пустые строки
    bool next();
    /// Список удаленных переменных
    QSet<int> removedVariables() const;
    /// Возвращает конструктор базисов
    MatrixBasisCreator<double> *creator() const;
protected:
    bool createFirstBasis();
    bool increment();
    void errorCorrention();
    void removeUnusedVariableAndRows();
protected:
    QStack<int> _basis;
    QSet<int> _removedVariables;
    MatrixBasisCreator<double> *_creator;
};

#endif // BASISITERATOR_H
