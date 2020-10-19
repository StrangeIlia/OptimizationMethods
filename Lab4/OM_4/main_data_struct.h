#ifndef MAINDATASTRUCT_H
#define MAINDATASTRUCT_H

#include <QHash>
#include <QMap>
#include <QSharedPointer>

struct VariantIndex {
    int row, column;
    double count;
};

class MainDataStruct;

typedef QSharedPointer<VariantIndex>    VariantIndexPtr;
typedef QSharedPointer<MainDataStruct>  MainDataStructPtr;

class MainDataStruct
{
public:
    MainDataStruct();

    typedef QHash<int, VariantIndexPtr>  RowsColumns;

    RowsColumns rows(int column) const;
    RowsColumns columns(int row) const;

    void insertIndex(int row, int column, double count);
    void removeIndex(int row, int column);

    void insertIndex(VariantIndexPtr ptr);
    void removeIndex(VariantIndexPtr ptr);

    VariantIndexPtr index(int row, int column) const;

    bool hasIndex(int row, int column) const;
    bool hasIndex(VariantIndexPtr index) const;

    QList<VariantIndexPtr> data() const;
    void clear();

private:
    QHash<int, RowsColumns> rowsData;
    QHash<int, RowsColumns> columnsData;
};



#endif // MAINDATASTRUCT_H
