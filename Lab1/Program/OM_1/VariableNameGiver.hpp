#ifndef VARIABLENAMEGIVER_H
#define VARIABLENAMEGIVER_H

#include "AbstractNameGiver.hpp"

class VariableNameGiver : public AbstractNameGiver
{
public:
    QString getName(int index) override {
        QString str = "x<span style=\" vertical-align:sub;\">";
        str += QString::number(index + 1);
        str += "</span>";
        return str;
    }
};

#endif // VARIABLENAMEGIVER_H
