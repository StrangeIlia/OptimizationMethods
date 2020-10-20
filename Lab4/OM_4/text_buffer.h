#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <QObject>
#include <QTextStream>
#include <QSharedPointer>

typedef QSharedPointer<QTextStream> QTextStreamPtr;

class TextBuffer : public QObject
{
    Q_OBJECT
public:
    explicit TextBuffer(QObject *parent = nullptr);

    QTextStreamPtr stream() const;

public slots:
    /// Вызывает сигнал flushed и очищает буфер
    void flush();

signals:
    void flushed(QString str);

private:
    QString _string;
    QTextStreamPtr _stream;
};

#endif // TEXTBUFFER_H
