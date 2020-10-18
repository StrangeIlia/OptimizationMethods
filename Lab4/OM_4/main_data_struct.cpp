#include "main_data_struct.h"

MainDataStruct::MainDataStruct() {

}

MainDataStruct::SortedData MainDataStruct::rows(int column) const {
    auto iter = columnsData.find(column);
    if(iter == rowsData.end()) return SortedData();
    return *iter;
}

MainDataStruct::SortedData MainDataStruct::columns(int row) const {
    auto iter = rowsData.find(row);
    if(iter == rowsData.end()) return SortedData();
    return *iter;
}

MainDataStruct::Range MainDataStruct::upper(int row, int column) const {
    auto columnIter = columnsData.find(row);
    if(columnIter == columnsData.end()) return Range(0, 0);
    return Range(columnIter->begin(), columnIter->upperBound(column));
}

MainDataStruct::Range MainDataStruct::lower(int row, int column) const {
    auto columnIter = columnsData.find(row);
    if(columnIter == columnsData.end()) return Range(0, 0);
    return Range(columnIter->upperBound(column), columnIter->end());
}

MainDataStruct::Range MainDataStruct::left(int row, int column) const {
    auto rowIter = rowsData.find(row);
    if(rowIter == rowsData.end()) return Range(0, 0);
    return Range(rowIter->begin(), rowIter->upperBound(column));
}

MainDataStruct::Range MainDataStruct::right(int row, int column) const {
    auto rowIter = rowsData.find(row);
    if(rowIter == rowsData.end()) return Range(0, 0);
    return Range(rowIter->upperBound(column), rowIter->end());
}

void MainDataStruct::insertIndex(int row, int column) {
    if(!hasIndex(row, column)) {
        VariantIndexPtr index(new VariantIndex);
        index->row = row;
        index->column = column;

        auto rowIter = rowsData.find(row);
        if(rowIter == rowsData.end()) {
            rowIter = rowsData.insert(row, SortedData());
        }
        rowIter->insert(column, index);

        auto columnIter = columnsData.find(column);
        if(columnIter == columnsData.end()) {
            columnIter = columnsData.insert(column, SortedData());
        }
        columnIter->insert(row, index);
    }
}

void MainDataStruct::removeIndex(int row, int column) {
    auto rowIter = rowsData.find(row);
    if(rowIter == rowsData.end()) return;
    rowIter->remove(column);
    if(rowIter->size() == 0)
        rowsData.remove(row);

    auto columnIter = columnsData.find(column);
    if(columnIter == columnsData.end()) return;
    columnIter->remove(row);
    if(columnIter->size() == 0)
        columnsData.remove(column);
}

VariantIndexPtr MainDataStruct::index(int row, int column) const {
    auto rowIter = rowsData.find(row);
    if(rowIter == rowsData.end()) return nullptr;
    auto columnIter = rowIter->find(column);
    if(columnIter == rowIter->end()) return nullptr;
    return *columnIter;
}

bool MainDataStruct::hasIndex(int row, int column) const {
    return !index(row, column).isNull();
}

bool MainDataStruct::hasIndex(VariantIndexPtr index) const {
    if(index == nullptr) return false;
    return hasIndex(index->row, index->column);
}

QList<VariantIndexPtr> MainDataStruct::data() const {
    QList<VariantIndexPtr> result;
    for(auto sortedColumns : rowsData) {
        for(auto value : sortedColumns) {
            result.push_back(value);
        }
    }
    return result;
}

void MainDataStruct::clear(){
    rowsData.clear();
    columnsData.clear();
}
