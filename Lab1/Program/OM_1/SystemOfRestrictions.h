#ifndef SYSTEMOFRESTRICTIONS_H
#define SYSTEMOFRESTRICTIONS_H

#include <QTableWidget>
#include <QStyledItemDelegate>

#include <math/matrix/MatrixOnRow.hpp>

#include "DelegateDouble.h"
#include "DelegateComboBox.h"

class SystemOfRestrictions : public QTableWidget
{
    Q_OBJECT
public:
    explicit SystemOfRestrictions(QWidget *parent = nullptr);

    enum Signs {
        EQUAL,
        MORE_OR_EQUAL,
        LESS_OR_EQUAL
    };

    QVector<Signs> getSigns();
    MatrixOnRow<double> getFreeMembers() const;
    MatrixOnRow<double> getCoefficients() const;


private:
    DelegateDouble* delegateDouble;
    DelegateComboBox* delegateComboBox;

    void addRow();
    void addVariable();

    void insertRowHeader(int index, QString value, QStyledItemDelegate* delegate = nullptr);
    void insertColumnHeader(int index, QString value, QStyledItemDelegate* delegate = nullptr);

    struct {int countRows = 0, countColumns = 0;} system;

private slots:
    void removeSelectedRow(int row);
    void removeSelectedColumn(int column);

    void checkUpdate(QTableWidgetItem* item);
};

#endif // SYSTEMOFRESTRICTIONS_H
