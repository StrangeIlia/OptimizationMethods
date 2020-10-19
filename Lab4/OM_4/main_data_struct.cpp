#include "main_data_struct.h"

MainDataStruct::MainDataStruct() {

}

MainDataStruct::RowsColumns MainDataStruct::rows(int column) const {
    auto iter = columnsData.find(column);
    if(iter == rowsData.end()) return RowsColumns();
    return *iter;
}

MainDataStruct::RowsColumns MainDataStruct::columns(int row) const {
    auto iter = rowsData.find(row);
    if(iter == rowsData.end()) return RowsColumns();
    return *iter;
}

void MainDataStruct::insertIndex(int row, int column, double count) {
    insertIndex(VariantIndexPtr(new VariantIndex{ row, column, count }));
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

void MainDataStruct::insertIndex(VariantIndexPtr ptr) {
    if(!hasIndex(ptr->row, ptr->column)) {
        auto rowIter = rowsData.find(ptr->row);
        if(rowIter == rowsData.end()) {
            rowIter = rowsData.insert(ptr->row, RowsColumns());
        }
        rowIter->insert(ptr->column, ptr);

        auto columnIter = columnsData.find(ptr->column);
        if(columnIter == columnsData.end()) {
            columnIter = columnsData.insert(ptr->column, RowsColumns());
        }
        columnIter->insert(ptr->row, ptr);
    }
}

void MainDataStruct::removeIndex(VariantIndexPtr ptr) {
    removeIndex(ptr->row, ptr->column);
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
