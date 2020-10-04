#include "mainwindow.h"

#include <QApplication>

#include "math/matrix/MatrixOnRow.hpp"
#include "DataProcessing.h"

#include "SystemOfRestrictions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    new DataProcessing(&w, &w);
    w.show();

    return a.exec();
}
