#ifndef ABSTRACTNAMEGIVER_H
#define ABSTRACTNAMEGIVER_H

#include <QObject>

/// Выдает для переменное, согласно ее номеру
class AbstractNameGiver {
public:
    virtual ~AbstractNameGiver(){}
    virtual QString getName(int index) = 0;
};

typedef AbstractNameGiver *AbstractNameGiverPtr;

Q_DECLARE_METATYPE(AbstractNameGiverPtr)

#endif // ABSTRACTNAMEGIVER_H
