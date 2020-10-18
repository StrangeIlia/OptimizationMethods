#ifndef MAINDATASTRUCT_H
#define MAINDATASTRUCT_H

#include <QHash>
#include <QMap>
#include <QSharedPointer>

struct VariantIndex {
    int row, column;
};

class MainDataStruct;

typedef QSharedPointer<VariantIndex>    VariantIndexPtr;
typedef QSharedPointer<MainDataStruct>  MainDataStructPtr;

class MainDataStruct
{
public:
    MainDataStruct();

    typedef QMap<int, VariantIndexPtr>  SortedData;
    typedef QPair<SortedData::const_iterator, SortedData::const_iterator>
                                        Range;

    SortedData rows(int column) const;
    SortedData columns(int row) const;

    Range upper(int row, int column) const;
    Range lower(int row, int column) const;

    Range left(int row, int column) const;
    Range right(int row, int column) const;

    void insertIndex(int row, int column);
    void removeIndex(int row, int column);

    VariantIndexPtr index(int row, int column) const;

    bool hasIndex(int row, int column) const;
    bool hasIndex(VariantIndexPtr index) const;

    QList<VariantIndexPtr> data() const;
    void clear();

private:
    QHash<int, SortedData> rowsData;
    QHash<int, SortedData> columnsData;
};



#endif // MAINDATASTRUCT_H
