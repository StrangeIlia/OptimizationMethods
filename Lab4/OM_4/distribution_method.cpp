#include "distribution_method.h"

DistributionMethod::DistributionMethod(MatrixPtr costs)  {
    _costs = costs;
}

//#include <QDebug>

bool DistributionMethod::oneStep(MainDataStructPtr data) {
//    qDebug() << "\nNew iteration";
    for(int i = 0; i != _costs->rows(); ++i) {
        for(int j = 0; j != _costs->columns(); ++j) {
            if(!data->hasIndex(i, j)) {
                auto cycle = createCycle(i, j, data);
                auto cost = cycleCost(cycle);

//                QString str = "(";
//                for(auto ptr : cycle) {
//                    str += "(" + QString::number(ptr->row) + ", " + QString::number(ptr->column) + "), ";
//                }
//                qDebug() << "(" << i << ", " << j << ") = " << cost << str;

                if(cost < 0) {
                    auto removedIndex = updateCycle(cycle);
                    data->removeIndex(removedIndex);
                    auto additionIndex = cycle.front();
                    data->insertIndex(additionIndex);
                    return true;
                }
            }
        }
    }

//    static bool swithBool = false;
//    if(!swithBool) {
//        swithBool = true;
//        auto cycle = createCycle(2, 1, data);
//        auto removedIndex = updateCycle(cycle);
//        data->removeIndex(removedIndex);
//        auto additionIndex = cycle.front();
//        data->insertIndex(additionIndex);
//        return true;
//    }

    return false;
}

QList<VariantIndexPtr> DistributionMethod::createCycle(int row, int column, MainDataStructPtr data) {
    class Processor {
    public:
         MainDataStructPtr data;
         QList<VariantIndexPtr> list;

         bool topDown() {
             const auto &curr = list.back();
             if(list.size() != 1 && curr->column == list.front()->column)
                 return true;

             auto rows = data->rows(curr->column);
             for(const auto &next : rows) {
                 if(next == curr) continue;

                 list.push_back(next);
                 if(leftRight()) return true;
                 list.pop_back();
             }

             return false;
         }

         bool leftRight() {
             const auto &curr = list.back();
             if(list.size() != 1 && curr->row == list.front()->row)
                 return true;

             auto columns = data->columns(curr->row);
             for(const auto &next : columns) {
                 if(next == curr) continue;

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
    for(int i = 1; i < cycle.size(); i += 2) {
        auto ptr = cycle[i];
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
