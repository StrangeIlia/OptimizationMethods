#include "text_buffer.h"

TextBuffer::TextBuffer(QObject *parent) : QObject(parent) {
    _stream.reset(new QTextStream(&_string));
}


QTextStreamPtr TextBuffer::stream() const {
    return _stream;
}

void TextBuffer::flush() {
    emit flushed(_string);
    _string.clear();
}
