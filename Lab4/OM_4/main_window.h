#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMainWindow>

#include "console.h"
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

private:
    Ui::MainWindow *ui;

    void initTables();
    void createConsole() const;

    void printText(QString text) const;
    void printHtml(QString html)  const;

    void closeEvent(QCloseEvent *event) override;

signals:
    void sendText(QString text) const;
    void sendHtml(QString html) const;
    void windowClosed() const;
};
#endif // MAINWINDOW_H
