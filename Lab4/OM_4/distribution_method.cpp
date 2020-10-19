#include "distribution_method.h"

DistributionMethod::DistributionMethod(MatrixPtr costs)  {
    _costs = costs;
}

bool DistributionMethod::oneStep(MainDataStructPtr data) {
    for(int i = 0; i != _costs->columns(); ++i) {
        for(int j = 0; j != _costs->columns(); ++j) {
            if(!data->hasIndex(i, j)) {
                auto cycle = createCycle(i, j, data);
                if(cycleCost(cycle) < 0) {
                    auto removedIndex = updateCycle(cycle);
                    data->removeIndex(removedIndex);
                    auto additionIndex = cycle.front();
                    data->insertIndex(additionIndex);
                    return true;
                }
            }
        }
    }
    return false;
}

QList<VariantIndexPtr> DistributionMethod::createCycle(int row, int column, MainDataStructPtr data) {
    class Processor {
    public:
         MainDataStructPtr data;
         QList<VariantIndexPtr> list;

         bool topDown() {
             const auto &curr = list.back();
             if(curr->column == list.front()->column)
                 return list.size() != 1;

             auto rows = data->rows(curr->column);
             for(const auto &next : rows) {
                 list.push_back(next);
                 if(leftRight()) return true;
                 list.pop_back();
             }

             return false;
         }

         bool leftRight() {
             const auto &curr = list.back();
             if(curr->row == list.front()->row)
                 return list.size() != 1;

             auto columns = data->columns(curr->row);
             for(const auto &next : columns) {
                 list.push_back(next);
                 if(topDown()) return true;
                 list.pop_back();
             }

             return false;
         }
    };

    Processor processor;
    processor.data = data;
    processor.list.push_front(VariantIndexPtr(new VariantIndex{row, column, 0}));

    if(processor.topDown())     return processor.list;
    if(processor.leftRight())   return processor.list;
    return QList<VariantIndexPtr>();
}

double DistributionMethod::cycleCost(QList<VariantIndexPtr> cycle) const {
    double cost = 0;
    for(int i = 0; i != cycle.size(); ++i) {
        auto ptr = cycle[i];
        auto value = _costs->cell(ptr->row, ptr->column);
        if(i % 2 == 0)  cost += value;
        else            cost -= value;
    }
    return cost;
}

VariantIndexPtr DistributionMethod::updateCycle(QList<VariantIndexPtr> cycle) {
    VariantIndexPtr index;
    double minValue = std::numeric_limits<double>::max();
    for(auto ptr : cycle) {
        if(ptr->count < minValue) {
            index = ptr;
            minValue = ptr->count;
        }
    }

    for(int i = 0; i != cycle.size(); ++i) {
        auto ptr = cycle[i];
        if(i % 2 == 0)  ptr->count += minValue;
        else            ptr->count -= minValue;
    }

    return index;
}
