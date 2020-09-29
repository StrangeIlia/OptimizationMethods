#include "ColumnNameChanger.h"

const QString ColumnNameChanger::HeaderField = "NameGiver";
const QString Group = "Group";
const QString NameGiver = "NameGiver";

ColumnNameChanger::ColumnNameChanger(QAbstractItemModel *model, QObject *parent) : QObject(parent)
{
    trackedModel = model;
    connect(trackedModel, SIGNAL(headerDataChanged(Qt::Orientation, int, int)), this, SLOT(nameChange(Qt::Orientation, int, int)));
}

void ColumnNameChanger::nameChange(Qt::Orientation orientation, int first, int last) {
    if(orientation == Qt::Orientation::Vertical) return;
    QMap<AbstractNameGiverPtr, int> groupAndIndex;
    for(int i = first; i <= last; ++i) {
        QVariant variant = trackedModel->headerData(i, orientation, HeaderInfo::TypeRole);
        if(variant.isValid()) {
            HeaderInfo info = variant.value<QVariantMap>();
            if(info.contains(HeaderField)) {
                AbstractNameGiverPtr nameGiver = info[HeaderField].value<AbstractNameGiverPtr>();
                if(nameGiver != nullptr)
                    groupAndIndex.insertMulti(nameGiver, i);
            }
        }
    }
    if(groupAndIndex.empty()) return;

    for(int i = 0; i != first; ++i) {
        QVariant variant = trackedModel->headerData(i, orientation, HeaderInfo::TypeRole);
        if(variant.isValid()) {
            HeaderInfo info = variant.value<QVariantMap>();
            if(info.contains(HeaderField)) {
                AbstractNameGiverPtr nameGiver = info[HeaderField].value<AbstractNameGiverPtr>();
                if(groupAndIndex.contains(nameGiver))
                    groupAndIndex.insertMulti(nameGiver, i);
            }
        }
    }

    for(int i = last + 1; i != trackedModel->columnCount(); ++i) {
        QVariant variant = trackedModel->headerData(i, orientation, HeaderInfo::TypeRole);
        if(variant.isValid()) {
            HeaderInfo info = variant.value<QVariantMap>();
            if(info.contains(HeaderField)) {
                AbstractNameGiverPtr nameGiver = info[HeaderField].value<AbstractNameGiverPtr>();
                if(groupAndIndex.contains(nameGiver))
                    groupAndIndex.insertMulti(nameGiver, i);
            }
        }
    }

    /// Не нашел способа определить, какие именно данные изменились, поэтому отключаю прием события
    disconnect(trackedModel, SIGNAL(headerDataChanged(Qt::Orientation, int, int)), this, SLOT(nameChange(Qt::Orientation, int, int)));

    auto listKeys = groupAndIndex.uniqueKeys();
    for(int i = 0; i != listKeys.size(); ++i) {
        auto pairIter = groupAndIndex.equal_range(listKeys[i]);
        for(; pairIter.first != pairIter.second; ++pairIter.first)
            trackedModel->setHeaderData(pairIter.first.value(), orientation, pairIter.first.key()->getName(pairIter.first.value()));
    }

    /// А затем снова включаю
    connect(trackedModel, SIGNAL(headerDataChanged(Qt::Orientation, int, int)), this, SLOT(nameChange(Qt::Orientation, int, int)));
}
