#ifndef OPTIONKEYS_H
#define OPTIONKEYS_H

#include <QObject>

class OptionKeys : public QObject
{
    Q_OBJECT
public:
    explicit OptionKeys(QObject* parent = 0);

    static const QString Torchlight2SharedStashFile;
    static const QString StashManagerFolder;
};

#endif // OPTIONKEYS_H
