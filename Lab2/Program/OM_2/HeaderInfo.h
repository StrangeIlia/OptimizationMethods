#ifndef HEADERINFO_H
#define HEADERINFO_H

#include <QMap>
#include <QVariant>

/// Содержит пользовательскую информацию о заголовке
class HeaderInfo : public QVariantMap
{
public:
    static const int TypeRole = 1000;

    static const QString GROUP;
    static const QString ALLOW_DELETION;
    static const QString ALLOW_EXTENSION;

    /// Разрешения на расширение
    enum AllowExtension{
        /// Отключить
        Disable = 0,
        /// Разрешено добавлять элементы с меньшим индексом
        Below = 1,
        /// Разрешено добавлять элементы с большим индексом
        Above = 2
    };

    explicit HeaderInfo();
    HeaderInfo(HeaderInfo&& info);
    HeaderInfo(const HeaderInfo& info);
    HeaderInfo(QMap<QString, QVariant>&& map);
    HeaderInfo(const QMap<QString, QVariant>& map);
    HeaderInfo(int group, AllowExtension extension, bool deletion);

    int group() const;
    bool deletion() const;
    AllowExtension extension() const;

    void setGroup(int value);
    void setDeletion(bool value);
    void setExtension(AllowExtension value);
};

Q_DECLARE_METATYPE(HeaderInfo)
Q_DECLARE_METATYPE(HeaderInfo::AllowExtension)

#endif // HEADERINFO_H
