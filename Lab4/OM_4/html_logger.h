#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QSharedPointer>

#include "text_buffer.h"

class HtmlLogger;

typedef QSharedPointer<HtmlLogger>  HtmlLoggerPtr;

class HtmlLogger
{
public:
    HtmlLogger(QTextStreamPtr stream);

    QTextStream &out();
    void message(QString message);

protected:
    QTextStreamPtr _stream;
};

#endif // LOGGER_H
