#ifndef DELEGATECONTROLLER_H
#define DELEGATECONTROLLER_H

#include <QTableWidget>

class DelegateController : public QObject
{
    Q_OBJECT
public:
    DelegateController(QTableWidget *widget, QObject *parent);
private slots:
    void rowsInserted(const QModelIndex &parent, int first, int last);
    void rowsRemoved(const QModelIndex &parent, int first, int last);
    void columnsInserted(const QModelIndex &parent, int first, int last);
    void columnsRemoved(const QModelIndex &parent, int first, int last);
private:
    QTableWidget *_trackedWidget;
};

#endif // DELEGATECONTROLLER_H
