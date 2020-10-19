#include "data_handler.h"

DataHandler::DataHandler() {

}

DataHandler::~DataHandler() {

}

void DataHandler::init(MainDataStructPtr /*data*/) {

}

void DataHandler::processing(MainDataStructPtr data) {
    init(data);
    while(oneStep(data));
}
