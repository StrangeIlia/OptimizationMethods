#ifndef RESULTFORM_H
#define RESULTFORM_H

#include <QWidget>
#include <QTextDocument>

namespace Ui {
class ResultForm;
}

class ResultForm : public QWidget
{
    Q_OBJECT

public:
    explicit ResultForm(QWidget *parent = nullptr);
    ~ResultForm();

    QTextDocument* getText() const;
    void setText(QTextDocument* str);

private:
    Ui::ResultForm *ui;
};

#endif // RESULTFORM_H
