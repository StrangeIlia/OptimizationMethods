#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QTextDocument>

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);
    ~Console();

public slots:
    void printText(QString text);
    void printHtml(QString html);
    void clear();

private:
    Ui::Console *ui;

    void closeEvent(QCloseEvent *event) override;

signals:
    void closed();
};

#endif // CONSOLE_H
