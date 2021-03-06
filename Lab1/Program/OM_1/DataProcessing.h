#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "mainwindow.h"
#include "ResultForm.h"
#include "BasisIterator.h"
#include "VariableNameGiver.hpp"

#include <QTextStream>

class DataProcessing : public QObject
{
    Q_OBJECT
public:
    DataProcessing(MainWindow *trackedWindow, QObject *parent);
private slots:
    void startProcessing();
private:
   MainWindow *trackedWindow;
};

#endif // CALCULATOR_H
