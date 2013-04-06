#ifndef OPTIONCOLLECTION_H
#define OPTIONCOLLECTION_H

#include <QObject>
#include <QHash>
#include <QtXml/QDomElement>
#include <QXmlStreamWriter>

class OptionCollection : public QObject
{
    Q_OBJECT
public:
    explicit OptionCollection(QObject* parent = 0);

//    const QString operator [](QString key) const { return mOptions[key]; }
//    QString& operator [](QString key)
//    {
//        return mOptions[key];
//    }


    void Save(QXmlStreamWriter& writer);
    void Load(const QDomElement& element);

//public slots:
    void Set(QString key, QString value);
    QString Get(QString key) const
    {
        QString result = mOptions.value(key);
        return result;
    }

    bool HasOption(QString key) const { return mOptions.contains(key); }

signals:
    void OptionsChanged(QString key, QString value);

private:
    QHash<QString, QString> mOptions;
};

#endif // OPTIONCOLLECTION_H
