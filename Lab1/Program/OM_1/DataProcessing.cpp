#include "DataProcessing.h"

DataProcessing::DataProcessing(MainWindow *trackedWindow, QObject *parent) : QObject(parent)
{
    this->trackedWindow = trackedWindow;
    connect(trackedWindow, &MainWindow::dataEntered, this, &DataProcessing::startProcessing);
}

void DataProcessing::startProcessing() {
    /// Считываем целевую функию
    auto funct = trackedWindow->function();
    if(funct.intent == MainWindow::MIN) {
        for(auto& value : funct.coefficients) {
            value = -value;
        }
        funct.intent = MainWindow::MAX;
    }

    MatrixOnRow<double> mainMatrix;
    auto rows = trackedWindow->constrains();
    int countVariable = rows.front().coefficients.size();
    mainMatrix.setColumns(countVariable + 1);
    /// Считываем данные
    for(int i = 0; i != rows.size(); ++i) {
        const auto& row = rows[i];
        mainMatrix.insertRows(mainMatrix.rows(), 1);
        for(int j = 0; j != row.coefficients.size(); ++j) {
            mainMatrix(i, j) = row.coefficients[j];
        }
        mainMatrix(i, mainMatrix.columns() - 1) = row.freeMember;
    }
    /// Добавляем вспомогательные переменные
    for(int i = 0; i != rows.size(); ++i) {
        const auto& row = rows[i];
        if(row.relatationSign == MainWindow::Signs::EQUAL) continue;
        mainMatrix.insertColumns(mainMatrix.columns() - 1, 1);
        if(row.relatationSign == MainWindow::Signs::LESS_OR_EQUAL)
            mainMatrix(i, mainMatrix.columns() - 2) =  1;
        else
            mainMatrix(i, mainMatrix.columns() - 2) = -1;
    }
    funct.coefficients.resize(mainMatrix.columns() - 1, 0);
    countVariable = mainMatrix.columns() - 1;

    VariableNameGiver *nameGiver = new VariableNameGiver();
    QString resultString;
    QTextStream out(&resultString);
    BasisIterator iterator(&mainMatrix);
    /// Удаляем неиспользуемые переменные
    auto removedVariables = iterator.removedVariables().values();
    std::sort(removedVariables.begin(), removedVariables.end());
    for(int i = 0; i != removedVariables.size(); ++i)
        funct.coefficients.erase(funct.coefficients.begin() + removedVariables[i] - i);

    out << tr("<p>Целевая функция: z = ");
    out << funct.coefficients[0] << nameGiver->getName(0);
    for(int i = 1; i != funct.coefficients.size(); ++i) {
        auto value = funct.coefficients[i];
        if(std::abs(value) > std::numeric_limits<double>::epsilon()) {
            if(funct.coefficients[i] > 0) out << tr("+");
            out << funct.coefficients[i] << nameGiver->getName(i);
        }
    }

    QString secondString;
    out.setString(&secondString);

    out << tr("→ max</p><p>");
    out << tr("Cистема ограничений, приведенная к каноническому виду") << tr("</p><p>");
    out.setFieldWidth(10);
    for(int i = 0; i != mainMatrix.rows(); ++i) {
        for(int j = 0; j != mainMatrix.columns() - 1; ++j) {
            out << mainMatrix(i, j) << " ";
        }
        out.setFieldWidth(0);
        out << "| ";
        out.setFieldWidth(10);
        out << mainMatrix(i, mainMatrix.columns() - 1);
        out << tr("</p><p>");
    }
    out << tr("</p><p>");
    out.setFieldWidth(0);

    int basisCount = 0;
    int bearingBasisCount = 0;
    while (iterator.next()) {
        bool bearingBasis = true;
        for(int i = 0; i != mainMatrix.rows(); ++i) {
            if(mainMatrix(i, mainMatrix.columns() - 1) < -std::numeric_limits<double>::epsilon()) {
                bearingBasis = false;
            }
        }
        ++basisCount;
        out << tr("Базис №") << basisCount;
        if(bearingBasis) {
            ++bearingBasisCount;
            double summ = 0;
            for(int i = 0; i != mainMatrix.rows(); ++i) {
                int variableNumber = iterator.creator()->getBasis(i);
                summ += funct.coefficients[variableNumber] * mainMatrix(i, mainMatrix.columns() - 1);
            }
            out << tr(", являющийся опорным базисом под номером ") << bearingBasisCount;
            out << tr("</p><p>");
            out << tr("Значение целевой функции z = ") << summ;
        }
        out << tr("</p><p>");
        out.setFieldWidth(10);
        for(int i = 0; i != mainMatrix.rows(); ++i) {
            for(int j = 0; j != mainMatrix.columns() - 1; ++j) {
                out << mainMatrix(i, j) << " ";
            }
            out.setFieldWidth(0);
            out << "| ";
            out.setFieldWidth(10);
            out << mainMatrix(i, mainMatrix.columns() - 1);
            out << tr("</p><p>");
        }
        out << tr("</p><p>");
        out.setFieldWidth(0);
    }

    out << tr("</p>");

    delete nameGiver;

    secondString.replace(" ", "&#160;");
    resultString += secondString;

    auto resultForm = new ResultForm();
    resultForm->getText()->setHtml(resultString);
    resultForm->show();
}
