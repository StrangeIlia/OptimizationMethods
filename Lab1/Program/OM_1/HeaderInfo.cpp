#include "HeaderInfo.h"

const QString HeaderInfo::GROUP = "Group";
const QString HeaderInfo::ALLOW_DELETION = "AllowDeletion";
const QString HeaderInfo::ALLOW_EXTENSION = "AllowExtension";

HeaderInfo::HeaderInfo(){
    insert(GROUP, 0);
    insert(ALLOW_DELETION, false);
    insert(ALLOW_EXTENSION, Disable);
}

HeaderInfo::HeaderInfo(HeaderInfo&& info) : QMap<QString, QVariant>(info) {

}

HeaderInfo::HeaderInfo(const HeaderInfo& info) : QMap<QString, QVariant>(info){

}

HeaderInfo::HeaderInfo(QMap<QString, QVariant>&& map) : QMap<QString, QVariant>(map) {

}

HeaderInfo::HeaderInfo(const QMap<QString, QVariant>& map) : QMap<QString, QVariant>(map) {

}

int HeaderInfo::group() const {
    return value(GROUP).toInt();
}

bool HeaderInfo::deletion() const {
    return value(ALLOW_DELETION).toBool();
}

HeaderInfo::AllowExtension HeaderInfo::extension() const {
    return value(ALLOW_EXTENSION).value<AllowExtension>();
}

void HeaderInfo::setGroup(int value) {
    insert(GROUP, value);
}

void HeaderInfo::setDeletion(bool value) {
    insert(ALLOW_DELETION, value);
}

void HeaderInfo::setExtension(AllowExtension value) {
    insert(ALLOW_EXTENSION, value);
}
