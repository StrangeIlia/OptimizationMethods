#include "mainwindow.h"
#include "ui_mainwindow.h"

class RowNameGiver : public AbstractNameGiver {
public:
    QString getName(int index) override {
        return QString::number(index + 1);
    }
};

QString MainWindow::ColumnsType = "ColumnsType";
QString MainWindow::Variable = "Variable";
QString MainWindow::Relatation = "Relatation";
QString MainWindow::FreeMember = "FreeMembers";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->matrix->setHorizontalHeader(new LabelHeaderView(Qt::Orientation::Horizontal/*, Qt::AlignHCenter | Qt::AlignVCenter*/, ui->matrix));
    ui->matrix->setVerticalHeader(new LabelHeaderView(Qt::Orientation::Vertical/*, Qt::AlignLeft | Qt::AlignVCenter*/, ui->matrix));

    auto delegateDouble = new DelegateDouble(Qt::AlignRight | Qt::AlignVCenter, this);

    QVariantHash comboBoxMap;
    comboBoxMap.insert("=", Signs::EQUAL);
    comboBoxMap.insert("≤", Signs::LESS_OR_EQUAL);
    comboBoxMap.insert("≥", Signs::MORE_OR_EQUAL);
    auto delegateComboBox = new DelegateComboBox(comboBoxMap, this);

    ui->function->setItemDelegate(delegateDouble);

    auto controller = new SizeController(ui->matrix, this);
    new ColumnDelegateCloner(controller, this);
    /// Перемещаем делегаты, ибо Qt тупенький и сам этого не делает
    new DelegateController(ui->matrix, this);

    controller->setMinRowCount(1);
    controller->setMinColumnCount(1);

    ui->matrix->insertRow(0);
    ui->matrix->insertRow(1);
    ui->matrix->insertColumn(0);
    ui->matrix->insertColumn(0);
    ui->matrix->insertColumn(0);
    ui->matrix->insertColumn(0);
    ui->function->insertColumn(0);
    ui->function->insertColumn(0);

    new SizeSynchronizer(ui->matrix->model(), ui->function->model(), false, true, this);

    auto variableNameGiver = new VariableNameGiver();
    columnsNameGiver = variableNameGiver;

    ui->matrix->setHorizontalHeaderItem(0, new QTableWidgetItem(variableNameGiver->getName(0)));
    ui->matrix->setHorizontalHeaderItem(1, new QTableWidgetItem(variableNameGiver->getName(1)));
    ui->matrix->setHorizontalHeaderItem(2, new QTableWidgetItem("Отношение"));
    ui->matrix->setHorizontalHeaderItem(3, new QTableWidgetItem("Свободный коэффициент"));

    ui->matrix->setItemDelegateForColumn(0, delegateDouble);
    ui->matrix->setItemDelegateForColumn(1, delegateDouble);
    ui->matrix->setItemDelegateForColumn(2, delegateComboBox);
    ui->matrix->setItemDelegateForColumn(3, delegateDouble);

    new ColumnNameChanger(ui->matrix->model(), this);

    HeaderInfo variableInfo;
    variableInfo[ColumnNameChanger::HeaderField] = QVariant::fromValue((AbstractNameGiverPtr)variableNameGiver);
    variableInfo.setDeletion(true);
    variableInfo.setExtension(HeaderInfo::Above);
    variableInfo[ColumnsType] = Variable;
    ui->matrix->horizontalHeaderItem(0)->setData(HeaderInfo::TypeRole, variableInfo);
    ui->matrix->horizontalHeaderItem(1)->setData(HeaderInfo::TypeRole, variableInfo);


    HeaderInfo relatationInfo;
    relatationInfo.setGroup(1);
    relatationInfo[ColumnsType] = Relatation;
    ui->matrix->horizontalHeaderItem(2)->setData(HeaderInfo::TypeRole, relatationInfo);

    HeaderInfo freeMemberInfo;
    freeMemberInfo.setGroup(2);
    freeMemberInfo[ColumnsType] = FreeMember;
    ui->matrix->horizontalHeaderItem(3)->setData(HeaderInfo::TypeRole, freeMemberInfo);

    auto rowsNameGiver = new RowNameGiver();
    this->rowsNameGiver = rowsNameGiver;

    HeaderInfo rowsInfo;
    rowsInfo[ColumnNameChanger::HeaderField] = QVariant::fromValue((AbstractNameGiverPtr) rowsNameGiver);
    rowsInfo.setDeletion(true);
    rowsInfo.setExtension(HeaderInfo::Above);
    ui->matrix->setVerticalHeaderItem(0, new QTableWidgetItem(rowsNameGiver->getName(0)));
    ui->matrix->setVerticalHeaderItem(1, new QTableWidgetItem(rowsNameGiver->getName(1)));
    ui->matrix->verticalHeaderItem(0)->setData(HeaderInfo::TypeRole, rowsInfo);
    ui->matrix->verticalHeaderItem(1)->setData(HeaderInfo::TypeRole, rowsInfo);

    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui->function->insertRow(0);
    ui->function->verticalHeader()->setVisible(false);
    ui->function->horizontalHeader()->setVisible(false);
    ui->function->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->function->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    ui->comboBox->insertItem(0, "→ max", FunctIntent::MAX);
    ui->comboBox->insertItem(0, "→ min", FunctIntent::MIN);

    connect(ui->pushButton, &QAbstractButton::clicked, this, &MainWindow::dataEntered);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete columnsNameGiver;
    delete rowsNameGiver;
}

MainWindow::ObjectiveFunction MainWindow::function() const {
    ObjectiveFunction result;
    auto table = ui->function;
    result.coefficients.reserve(table->columnCount());
    for(int i = 0; i != table->columnCount(); ++i) {
        QVariant variant;
        if(table->item(0, i) != nullptr)
            variant = table->item(0, i)->data(0);
        if(variant.isValid() && variant.canConvert<double>()) {
            double value = variant.value<double>();
            result.coefficients.push_back(value);
        } else {
            result.coefficients.push_back(0);
        }
    }

    QVariant variant = ui->comboBox->currentData();
    if(variant.isValid() && variant.canConvert<FunctIntent>())
        result.intent = variant.value<FunctIntent>();
    else
        result.intent = FunctIntent::MAX;

    return result;
}

std::vector<MainWindow::ConstraintRow> MainWindow::constrains() const {
    std::vector<ConstraintRow> result;
    auto table = ui->matrix;
    result.resize(table->rowCount());
    for(int j = 0; j != table->columnCount(); ++j) {
        auto header = table->horizontalHeaderItem(j);
        auto variantHeaderData = header->data(HeaderInfo::TypeRole);
        if(variantHeaderData.isValid() && variantHeaderData.canConvert<QVariantMap>()) {
            auto headerData = variantHeaderData.value<QVariantMap>();
            if(headerData.contains(ColumnsType) && headerData[ColumnsType].canConvert<QString>()) {
                QString type = headerData[ColumnsType].value<QString>();
                if(type == Variable) {
                    for(int i = 0; i != table->rowCount(); ++i) {
                        QVariant variant;
                        auto item = table->item(i, j);
                        if(item != nullptr) variant = item->data(0);
                        if(variant.isValid() && variant.canConvert<double>())
                            result[i].coefficients.push_back(variant.value<double>());
                        else
                            result[i].coefficients.push_back(0);
                    }
                } else if(type == Relatation) {
                    for(int i = 0; i != table->rowCount(); ++i) {
                        QVariant variant;
                        auto item = table->item(i, j);
                        if(item != nullptr) variant = item->data(Qt::UserRole);
                        if(variant.isValid() && variant.canConvert<Signs>())
                            result[i].relatationSign = variant.value<Signs>();
                        else
                            result[i].relatationSign = Signs::EQUAL;
                    }
                } else if(type == FreeMember) {
                    for(int i = 0; i != table->rowCount(); ++i) {
                        QVariant variant;
                        auto item = table->item(i, j);
                        if(item != nullptr) variant = item->data(0);
                        if(variant.isValid() && variant.canConvert<double>())
                            result[i].freeMember = variant.value<double>();
                        else
                            result[i].freeMember = 0;
                    }
                } else continue;
            }
        }
    }
    return result;
}


//void MainWindow::setFunction(const ObjectiveFunction& funct) {
//    if(funct.coefficients.size() != ui->function->columnCount())
//        return; // thrown
//    auto table = ui->function;
//    for(int i = 0; i != table->columnCount(); ++i) {
//        QVariant variant;
//        if(table->item(0, i) != nullptr)
//            variant = table->item(0, i)->data(0);
//        if(variant.isValid() && variant.canConvert<double>()) {
//            double value = variant.value<double>();
//            result.coefficients.push_back(value);
//        } else {
//            result.coefficients.push_back(0);
//        }
//    }
//}

//void MainWindow::setConstrains(const std::vector<ConstraintRow> &rows) {

//}
