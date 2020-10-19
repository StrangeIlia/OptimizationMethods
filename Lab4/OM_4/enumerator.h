#ifndef CALCULATIONITERATOR_H
#define CALCULATIONITERATOR_H

#include <QSharedPointer>

class Enumerator
{
public:
    Enumerator();

    virtual bool reset();
    virtual bool moveNext() = 0;
};

typedef QSharedPointer<Enumerator> EnumeratorPtr;

#endif // CALCULATIONITERATOR_H
