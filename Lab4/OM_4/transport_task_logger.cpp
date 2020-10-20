#include "transport_task_logger.h"

TransportTaskLogger::TransportTaskLogger(QTextStreamPtr stream) : HtmlLogger(stream) {

}

bool TransportTaskLogger::isValid() const {
    if(_comsumers == nullptr)   return false;
    if(_suppliers == nullptr)   return false;
    if(_costs == nullptr)       return false;
    if(_comsumers->columns() != _costs->columns())  return false;
    if(_suppliers->columns() != _costs->rows())     return false;
    return true;
}

MatrixPtr TransportTaskLogger::costs() const {
    return _costs;
}

MatrixPtr TransportTaskLogger::suplliers() const {
    return _suppliers;
}

MatrixPtr TransportTaskLogger::comsumers() const {
    return _comsumers;
}

void TransportTaskLogger::setCosts(MatrixPtr ptr) {
    _costs = ptr;
}

void TransportTaskLogger::setSuppliers(MatrixPtr ptr) {
    _suppliers = ptr;
}

void TransportTaskLogger::setComsumers(MatrixPtr ptr) {
    _comsumers = ptr;
}

void TransportTaskLogger::printTransportTable(MainDataStructPtr data) {
    auto &out = *_stream;
    if(!isValid())  {
        out << "Logger is not valid<br>";
        return;
    }

    auto printCell = [&out] (double value, double cost) {
        out << "<table width=\"100%\" height=\"100%\" border=\"1\" cellpadding=\"4\" cellspacing=\"0\">";
        out << "<tr><td colspan=\"2\" style=\"border-bottom: 0;\">";
        if(std::abs(value) > std::numeric_limits<double>::epsilon())    out << value;
        else                                                            out << " ";
        out << "</td></tr><tr><td style=\"border-top: 0; border-right: 0;\"></td><td><span style=\"vertical-align:sub;\">";
        //
        out << cost;
        out << "</span></td></tr></table>";
    };

    out << "<table border=\"1\" cellpadding=\"0\" cellspacing=\"0\"><tr><td/>";
    for(int i = 0; i != _comsumers->columns(); ++i) {
        out << "<td>" << _comsumers->cell(0, i) << "</td>";
    }
    out << "</tr>";

    for(int i = 0; i != _suppliers->columns(); ++i) {
        out << "<tr><td>" << _suppliers->cell(0, i) << "</td>";
        auto row = data->columns(i);
        for(int j = 0; j != _comsumers->columns(); ++j) {
            auto value = row.contains(j) ? row[j]->count : 0;
            out << "<td>";
            printCell(value, _costs->cell(i, j));
            out << "</td>";
        }
        out << "</tr>";
    }
    out << "</table>";
}
