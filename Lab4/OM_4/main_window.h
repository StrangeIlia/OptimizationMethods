#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMessageBox>
#include <QMainWindow>
#include <QTableWidget>

#include "console.h"
#include "calculator.h"
#include "delegate_double.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool checkInputData() const;

private slots:
    void initTestVariant();
    void calculate();

private:
    Ui::MainWindow *ui;

    void initTables();
    void initComboboxesAndActiveButton();
    void createConsole() const;

    void printText(QString text) const;
    void printHtml(QString html) const;

    static MatrixPtr matrix(QTableWidget *widget);

    void closeEvent(QCloseEvent *event) override;

signals:
    void sendText(QString text) const;
    void sendHtml(QString html) const;
    void sendData(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs) const;
    void windowClosed() const;
};

Q_DECLARE_METATYPE(MatrixPtr)

#endif // MAINWINDOW_H
