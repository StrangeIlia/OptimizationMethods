#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "DelegateDouble.h"
#include "DelegateComboBox.h"

#include "SizeController.h"
#include "SizeSynchronizer.h"
#include "ColumnNameChanger.h"
#include "ColumnDelegateCloner.h"

#include "VariableNameGiver.hpp"
#include "DelegateController.h"

#include "LabelHeaderView.h"

#include "math/matrix/MatrixOnRow.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum Signs {
        EQUAL,
        MORE_OR_EQUAL,
        LESS_OR_EQUAL
    };

    enum FunctIntent {
        MIN,
        MAX
    };

    struct ConstraintRow{
        std::vector<double> coefficients;
        double freeMember;
        Signs relatationSign;
    };

    struct ObjectiveFunction {
        std::vector<double> coefficients;
        FunctIntent intent;
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ObjectiveFunction function() const;
    std::vector<ConstraintRow> constrains() const;

    void setFunction(const ObjectiveFunction& funct);
    void setConstrains(const std::vector<ConstraintRow>&);

signals:
    void dataEntered();
private:
    Ui::MainWindow *ui;
    AbstractNameGiver *rowsNameGiver;
    AbstractNameGiver *columnsNameGiver;

    static QString ColumnsType;
    static QString Variable;
    static QString Relatation;
    static QString FreeMember;
};

Q_DECLARE_METATYPE(MainWindow::Signs);
Q_DECLARE_METATYPE(MainWindow::FunctIntent);

#endif // MAINWINDOW_H
