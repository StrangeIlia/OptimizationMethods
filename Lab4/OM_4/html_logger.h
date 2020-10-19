#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QSharedPointer>

class HtmlLogger;

typedef QSharedPointer<QTextStream> QTextStreamPtr;
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
