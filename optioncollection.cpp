#include "optioncollection.h"

OptionCollection::OptionCollection(QObject* parent)
    : QObject(parent)
{

}

void OptionCollection::Set(QString key, QString value)
{
    mOptions[key] = value;
    emit OptionsChanged(key, value);
}

void OptionCollection::Save(QXmlStreamWriter& writer)
{
    QHashIterator<QString, QString> it(mOptions);
    while(it.hasNext())
    {
        it.next();
        writer.writeStartElement(it.key());
        writer.writeCharacters(it.value());
        writer.writeEndElement();
    }
}

void OptionCollection::Load(const QDomElement& element)
{
    QDomNodeList childNodes = element.childNodes();
    for(int i = 0; i < childNodes.count(); ++i)
    {
        QDomNode next = childNodes.at(i);

        if (!next.isNull() && next.isElement())
        {
            QDomElement nextElement = next.toElement();
            mOptions[nextElement.nodeName()] = nextElement.text();
        }
    }
}
