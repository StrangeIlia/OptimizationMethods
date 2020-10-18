#include "distribution_method.h"

DistributionMethod::DistributionMethod(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs)  {
    _suppliers = suppliers;
    _comsumers = comsumers;
    _costs = costs;
}

void DistributionMethod::processing(MainDataStructPtr data) {
    Matrix suppliers = *_suppliers;
    Matrix comsumers = *_comsumers;

    suppliers.setSize(1, suppliers.rows() * suppliers.columns());
    comsumers.setSize(1, comsumers.rows() * comsumers.columns());

    bool reqieredContinue = true;
    while(reqieredContinue) {
        reqieredContinue = false;
        for(int i = 0; i != suppliers.columns(); ++i) {
            for(int j = 0; j != comsumers.columns(); ++j) {
                if()
            }
        }
    }
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
    processor.list.push_front(VariantIndexPtr(new VariantIndex{row, column}));

    if(processor.topDown())     return processor.list;
    if(processor.leftRight())   return processor.list;
    return QList<VariantIndexPtr>();
}

double DistributionMethod::cycleCost(QList<VariantIndexPtr> cycle) const {
    double cost = 0;
    for(auto ptr : cycle) {
        cost += _costs->cell(ptr->row, ptr->column);
    }
    return cost;
}
