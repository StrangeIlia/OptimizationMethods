#include "html_logger.h"

HtmlLogger::HtmlLogger(QTextStreamPtr stream) : _stream(stream) {

}

QTextStream &HtmlLogger::out() {
    return *_stream;
}

void HtmlLogger::message(QString message) {
    out() << message << "<br>";
}
