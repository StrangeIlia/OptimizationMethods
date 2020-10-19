#include "data_processing.h"

DataProcessing::DataProcessing() {

}

DataProcessing::~DataProcessing() {

}

void DataProcessing::init(MainDataStructPtr /*data*/) {

}

void DataProcessing::processing(MainDataStructPtr data) {
    init(data);
    while(oneStep(data));
}
