#ifndef TORCHLIGHT2STASHCONVERTER_H
#define TORCHLIGHT2STASHCONVERTER_H

#include <QObject>
#include <iostream>

#define BASE_HEADER_SIZE 5
#define CRC_SIZE 4
#define FOOTER_SIZE 4

class Torchlight2StashConverter : public QObject
{
public:
    explicit Torchlight2StashConverter(QObject* parent = 0);

    static bool DescrambleFile(QString inputFilePath, QString outputFilePath);
    static bool ScrambleFile(QString inputFilePath, QString outputFilePath);
};

#endif // TORCHLIGHT2STASHCONVERTER_H
