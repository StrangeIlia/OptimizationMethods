#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QSet>
#include <QList>
#include <QObject>

#include "potential_method.h"
#include "distribution_method.h"

#include "least_cost_method.h"
#include "nortwest_corner_method.h"

#include "calculation_task.h"
#include "transport_task_logger.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    Calculator(QObject *parent = nullptr);
    ~Calculator();

    TextBuffer *buffer() const;

    static QSet<QString> initMethods();
    static QSet<QString> calculateMethods();

public slots:
    void calculate(MatrixPtr suppliers, MatrixPtr comsumers, MatrixPtr costs);
    bool setInitMethod(const QString &str);
    bool setCalcMethod(const QString &str);

signals:
    void finished();

private:
    QString _init;
    QString _calculate;
    TextBuffer *_buffer;

    static QSet<QString> _initMethods;
    static QSet<QString> _calculateMethods;
};

#endif // CALCULATOR_H
