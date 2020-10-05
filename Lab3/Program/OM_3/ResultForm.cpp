#include "ResultForm.h"
#include "ui_ResultForm.h"

ResultForm::ResultForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultForm)
{
    ui->setupUi(this);
}

ResultForm::~ResultForm()
{
    delete ui;
}

QTextDocument* ResultForm::getText() const {
    return ui->textEdit->document();
}

void ResultForm::setText(QTextDocument* doc) {
    ui->textEdit->setDocument(doc);
}
