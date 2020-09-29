#ifndef MATRIXONROW_HPP
#define MATRIXONROW_HPP

#include <vector>
#include <stdexcept>

/// Qt не любит шаблоны, поэтому реализации прямо в файле с заголовками
template<class _BaseType, class _IndexType = int>
class MatrixOnRow
{
public:
    typedef     _BaseType       BaseType;
    typedef     _IndexType      IndexType;
private:
    int rows, columns;
    std::vector<BaseType> matrix;
public:
    MatrixOnRow();
    MatrixOnRow(MatrixOnRow&& othen);
    MatrixOnRow(const MatrixOnRow& othen);
    /// Задает матрицу заданного размера, раполняет базовым значением типа
    MatrixOnRow(IndexType _rows, IndexType _columns);

    inline MatrixOnRow& operator = (MatrixOnRow&& othen);
    inline MatrixOnRow& operator = (const MatrixOnRow& othen);

    /// Вставляет в матрицу count строк, заполнениых нулями
    inline void insertRows(IndexType index, IndexType count);
    /// Вставляет в матрицу count столбцов, заполнениых нулями
    inline void insertColumns(IndexType index, IndexType count);
    /// Удаляет из матрицы строки с first по last включительно ([first; last])
    inline void removeRows(IndexType first, IndexType last);
    /// Удаляет из матрицы строки с first по last включительно ([first; last])
    inline void removeColumns(IndexType first, IndexType last);

    /// Изменяет число строк, сохранение данных не гарантировано
    inline void setRows(IndexType _rows);
    /// Изменяет число столбцов, сохранение данных не гарантировано
    inline void setColumns(IndexType _columns);
    /// Изменяет число строк и столбцов, сохранение данных не гарантировано
    inline void setSize(IndexType _rows, IndexType _columns);

    inline IndexType getRows() const;
    inline IndexType getColumns() const;

    inline void setCell(IndexType i, IndexType j, const BaseType&);
    inline const BaseType& getCell(IndexType i, IndexType j) const;

    inline BaseType& operator() (IndexType i, IndexType j);
    inline const BaseType& operator() (IndexType i, IndexType j) const;

    /// Методы для рефлексии
    inline std::vector<BaseType>& getBaseRow();
    inline const std::vector<BaseType>& getBaseRow() const;
};

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>::MatrixOnRow() : rows(0), columns(0) {

}

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>::MatrixOnRow(MatrixOnRow&& othen) :
    rows(othen.rows), columns(othen.columns), matrix(std::move(othen.matrix))  {
    othen.rows = othen.columns = 0;
}

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>::MatrixOnRow(const MatrixOnRow& othen) :
    rows(othen.rows), columns(othen.columns), matrix(othen.matrix) {
}

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>::MatrixOnRow(IndexType _rows, IndexType _columns) {
    if(_rows < 0) throw std::invalid_argument("MatrixOnRow::setSize: The number of rows cannot be negative");
    if(_columns < 0) throw std::invalid_argument("MatrixOnRow::setSize: The number of columns cannot be negative");
    rows = _rows;
    columns = _columns;
    matrix.resize(rows * columns);
}

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>& MatrixOnRow<BaseType, IndexType>::operator = (MatrixOnRow&& othen) {
    rows = othen.rows;
    columns = othen.columns;
    matrix = std::move(othen.matrix);
    othen.rows = othen.columns = 0;
}

template<class BaseType, class IndexType>
MatrixOnRow<BaseType, IndexType>& MatrixOnRow<BaseType, IndexType>::operator = (const MatrixOnRow& othen) {
    rows = othen.rows;
    columns = othen.columns;
    matrix = othen.matrix;
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::insertRows(IndexType index, IndexType count) {
    if(index < 0) throw std::invalid_argument("MatrixOnRow::insertRows: index не может быть отрицательным числом");
    if(index > rows) throw std::invalid_argument("MatrixOnRow::insertRows: index не может быть больше количества строк");
    if(count < 0) throw std::invalid_argument("MatrixOnRow::insertRows: count не может быть отрицательным числом");
    matrix.insert(index, count * columns, 0);
    rows += count;
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::insertColumns(IndexType index, IndexType count) {
    if(index < 0) throw std::invalid_argument("MatrixOnRow::insertColumns: index не может быть отрицательным числом");
    if(index > columns) throw std::invalid_argument("MatrixOnRow::insertColumns: index не может быть больше количества строк");
    if(count < 0) throw std::invalid_argument("MatrixOnRow::insertColumns: count не может быть отрицательным числом");
    matrix.reserve(matrix.size() + rows * count);
    columns += count;
    for(int i = 0; i != rows; ++i) {
        matrix.insert(i * columns + index, count, 0);
    }
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::removeRows(IndexType first, IndexType last) {
    if(last < first)  throw std::invalid_argument("MatrixOnRow::removeRows: last не может быть меньше first");
    if(first < 0) throw std::invalid_argument("MatrixOnRow::removeRows: first не может быть отрицательным числом");
    if(first > rows) throw std::invalid_argument("MatrixOnRow::removeRows: first не может быть больше количества строк");
    if(last < 0) throw std::invalid_argument("MatrixOnRow::removeRows: first не может быть отрицательным числом");
    if(last > rows) throw std::invalid_argument("MatrixOnRow::removeRows: first не может быть больше количества строк");
    matrix.erase(first * columns, (last + 1) * columns);
    rows -= last - first + 1;
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::removeColumns(IndexType first, IndexType last) {
    if(last < first)  throw std::invalid_argument("MatrixOnRow::removeColumns: last не может быть меньше first");
    if(first < 0) throw std::invalid_argument("MatrixOnRow::removeColumns: first не может быть отрицательным числом");
    if(first > columns) throw std::invalid_argument("MatrixOnRow::removeColumns: first не может быть больше количества столбцов");
    if(last < 0) throw std::invalid_argument("MatrixOnRow::removeColumns: first не может быть отрицательным числом");
    if(last > columns) throw std::invalid_argument("MatrixOnRow::removeColumns: first не может быть больше количества столбцов");
    columns -= first - last + 1;
    for(int i = 0; i != rows; ++i) {
        matrix.erase(i * columns + first, last + 1);
    }
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::setRows(IndexType _rows) {
    setSize(_rows, columns);
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::setColumns(IndexType _columns) {
    setSize(rows, _columns);
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::setSize(IndexType _rows, IndexType _columns) {
    if(rows == _rows && columns == _columns) return;
    if(_rows < 0) throw std::invalid_argument("MatrixOnRow::setSize: The number of rows cannot be negative");
    if(_columns < 0) throw std::invalid_argument("MatrixOnRow::setSize: The number of columns cannot be negative");
    rows = _rows;
    columns = _columns;
    matrix.resize(rows * columns);
}

template<class BaseType, class IndexType>
IndexType MatrixOnRow<BaseType, IndexType>::getRows() const {
    return rows;
}

template<class BaseType, class IndexType>
IndexType MatrixOnRow<BaseType, IndexType>::getColumns() const {
    return columns;
}

template<class BaseType, class IndexType>
void MatrixOnRow<BaseType, IndexType>::setCell(IndexType i, IndexType j, const BaseType& value) {
    matrix[i * columns + j] = value;
}

template<class BaseType, class IndexType>
const BaseType& MatrixOnRow<BaseType, IndexType>::getCell(IndexType i, IndexType j) const {
    return matrix[i * columns + j];
}

template<class BaseType, class IndexType>
BaseType& MatrixOnRow<BaseType, IndexType>::operator() (IndexType i, IndexType j) {
    return matrix[i * columns + j];
}

template<class BaseType, class IndexType>
const BaseType& MatrixOnRow<BaseType, IndexType>::operator() (IndexType i, IndexType j) const {
    return matrix[i * columns + j];
}

template<class BaseType, class IndexType>
std::vector<BaseType>& MatrixOnRow<BaseType, IndexType>::getBaseRow() {
    return matrix;
}

template<class BaseType, class IndexType>
const std::vector<BaseType>& MatrixOnRow<BaseType, IndexType>::getBaseRow() const {
    return matrix;
}

#endif // MATRIXONROW_HPP
