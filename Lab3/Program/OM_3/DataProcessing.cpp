#include "DataProcessing.h"

DataProcessing::DataProcessing(MainWindow *trackedWindow, QObject *parent) : QObject(parent)
{
    this->trackedWindow = trackedWindow;
    connect(trackedWindow, &MainWindow::dataEntered, this, &DataProcessing::startProcessing);
}

void DataProcessing::startProcessing() {
    MatrixOnRow<double> mainMatrix = readSimplexTable();
    VariableNameGiver *nameGiver = new VariableNameGiver();
    QString resultString = "<p>";
    QTextStream out(&resultString);
    SimpleMethodIterator<double> iterator(&mainMatrix);

    int indexFunct = mainMatrix.rows() - 1;
    int variableCount = mainMatrix.columns() - 1;

    out << tr("Целевая функция: z = ");

    bool reqPlus = false;
    for(int i = 0; i != variableCount; ++i) {
        auto value = -mainMatrix(indexFunct, i);
        if(std::abs(value) > std::numeric_limits<double>::epsilon()) {
            if(reqPlus && value > 0) out << tr("+ ");
            if(std::abs(value - 1) < std::numeric_limits<double>::epsilon())
                out << nameGiver->getName(i);
            else if(std::abs(value + 1) < std::numeric_limits<double>::epsilon())
                out << "-" << nameGiver->getName(i);
            else
                out << value << nameGiver->getName(i);
            reqPlus = true;
            out << " ";
        }
    }

    out << tr(" → max<br><br><br>");

    iterator.init();
    int simplexTableCount = 0;
    out.setRealNumberNotation(QTextStream::FixedNotation);
    do {
        ++simplexTableCount;
        out << tr("Симлекс-таблица №") << simplexTableCount << "\n";
        out << "<table border=\"1\" cellpadding=\"5\">";
        out << "<tr>";
        out << tr("<th>Базисные переменные</th><th>Свободные коэффициенты</th>");
        for(int i = 0; i != variableCount; ++i) {
            out << "<th>   " << nameGiver->getName(i) << "   </th>";
        }
        out << "</tr>";

        for(int i = 0; i != indexFunct; ++i) {
            out << "<tr><td aling=\"center\">" << nameGiver->getName(iterator.creator()->getBasis(i)) << "</td>";
            out << "<td aling=\"center\">" << mainMatrix.cell(i, variableCount) << "</td>";
            for(int j = 0; j != variableCount; ++j) {
                out << "<td aling=\"center\">" << mainMatrix.cell(i, j) << "</td>";
            }
            out << "</tr>";
        }

        out << "<tr><td aling=\"center\">z</td>";
        out << "<td aling=\"center\">" << mainMatrix.cell(indexFunct, variableCount) << "</td>";
        for(int j = 0; j != variableCount; ++j) {
            out << "<td aling=\"center\">"<< mainMatrix.cell(indexFunct, j) << "</td>";
        }
        out << "</tr>";
        out << "</table>";
        out << "<br><br><br>";
        out.setFieldWidth(0);
    } while(iterator.next());

    out << "<br>";

    delete nameGiver;

    resultString.replace("  ", " &#160;");
    resultString += "</p>";

    auto resultForm = new ResultForm();
    resultForm->getText()->setHtml(resultString);
    resultForm->show();
}

MatrixOnRow<double> DataProcessing::readSimplexTable() const {
    MatrixOnRow<double> result;
    result.insertRows(0, 1);

    /// Считываем целевую функию
    auto funct = trackedWindow->function();
    int countVariable = funct.coefficients.size();
    result.insertColumns(0, countVariable + 1);
    if(funct.intent == MainWindow::MAX) {
        for(int j = 0; j != funct.coefficients.size(); ++j) {
            result(0, j) = -funct.coefficients[j];
        }
    } else {
        for(int j = 0; j != funct.coefficients.size(); ++j) {
            result(0, j) = funct.coefficients[j];
        }
    }

    /// Считываем ограничения
    auto rows = trackedWindow->constrains();
    int rowsCount = rows.size();
    result.insertRows(0, rowsCount);
    for(int i = 0; i != rowsCount; ++i) {
        const auto& row = rows[i];
        for(int j = 0; j != row.coefficients.size(); ++j) {
            result(i, j) = row.coefficients[j];
        }
        result(i, countVariable) = row.freeMember;
    }

    /// Добавляем вспомогательные переменные
    for(int i = 0; i != rowsCount; ++i) {
        const auto& row = rows[i];
        if(row.relatationSign == MainWindow::Signs::EQUAL) continue;
        result.insertColumns(result.columns() - 1, 1);
        if(row.relatationSign == MainWindow::Signs::LESS_OR_EQUAL)
            result(i, result.columns() - 2) =  1;
        else
            result(i, result.columns() - 2) = -1;
    }

    /// Удаляем пустые строки
    for(int i = 0; i != rowsCount; ) {
        bool isEmptyRow = true;
        for(int j = 0; j != countVariable; ++j) {
            if(std::abs(result.cell(i, j)) > std::numeric_limits<double>::epsilon()) {
                isEmptyRow = false;
                break;
            }
        }
        if(isEmptyRow) {
            result.removeRows(i, i);
            --rowsCount;
        } else ++i;
    }

    /// Удаляем пустые колонки
    for(int j = 0; j != countVariable; ) {
        bool isEmptyColumn = true;
        for(int i = 0; i != rowsCount; ++i) {
            if(std::abs(result.cell(i, j)) > std::numeric_limits<double>::epsilon()) {
                isEmptyColumn = false;
                break;
            }
        }
        if(isEmptyColumn) {
            result.removeColumns(j, j);
            --countVariable;
        } else ++j;
    }

    /// Пока инициализации значения нет

    return result;
}
