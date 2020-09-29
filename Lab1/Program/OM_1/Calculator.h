#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "mainwindow.h"
#include "ResultForm.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    Calculator(MainWindow *trackedWindow, ResultForm *resultForm, QObject *parent);
private slots:
    void startProcessing();
private:
   MainWindow *trackedWindow;
   ResultForm *resultForm;
};

#endif // CALCULATOR_H
