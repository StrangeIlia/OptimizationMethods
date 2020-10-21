#include "console.h"
#include "ui_console.h"

Console::Console(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    connect(ui->clear, &QPushButton::clicked, [this] (bool /*ignored*/){
        this->clear();
    });
    connect(this, &Console::closed, this, &Console::deleteLater);
}

Console::~Console()
{
    delete ui;
}

void Console::printText(QString text) {
    QTextDocument doc;
    doc.setPlainText(text);
    printHtml(doc.toHtml());
}

void Console::printHtml(QString html) {
    if(ui->textEdit->toPlainText().size() == 0) {
        ui->textEdit->setHtml(html);
    } else {
        QString str = ui->textEdit->toHtml();
        str += "<br>-----------------------------------------------";
        ui->textEdit->setHtml(str + html);
    }
}

void Console::clear() {
    ui->textEdit->setHtml("");
}

void Console::closeEvent(QCloseEvent *event) {
    emit closed();
}
