#ifndef VARIABLENAMEGIVER_H
#define VARIABLENAMEGIVER_H

#include "AbstractNameGiver.hpp"

class VariableNameGiver : public AbstractNameGiver
{
    QString _base;

public:
    VariableNameGiver(QString base = "x") {
        _base = base;
    }

    QString getName(int index) override {
        QString str = _base + "<span style=\" vertical-align:sub;\">";
        str += QString::number(index + 1);
        str += "</span>";
        return str;
    }
};

#endif // VARIABLENAMEGIVER_H
