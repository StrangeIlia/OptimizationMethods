#include "Calculator.h"

Calculator::Calculator(MainWindow *trackedWindow, ResultForm *resultForm, QObject *parent) : QObject(parent)
{
    this->trackedWindow = trackedWindow;
    this->resultForm = resultForm;
    connect(trackedWindow, &MainWindow::dataEntered, this, &Calculator::startProcessing);
}

void Calculator::startProcessing() {

}
