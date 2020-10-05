#include "mainwindow.h"

#include <QApplication>

#include "math/matrix/MatrixOnRow.hpp"
#include "DataProcessing.h"

/// Просьба, не удалять первый столбец
/// (Qt почему - то не отлавливает сигнал именно для него)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    new DataProcessing(&w, &w);
    w.show();

    return a.exec();
}
