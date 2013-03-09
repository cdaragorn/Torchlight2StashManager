#ifndef TORCHLIGHT2STASHCONVERTER_H
#define TORCHLIGHT2STASHCONVERTER_H

#include <QObject>

#define BASE_HEADER_SIZE 5
#define CRC_SIZE 4
#define FOOTER_SIZE 4

class Torchlight2StashConverter : public QObject
{
public:
    explicit Torchlight2StashConverter(QObject* parent = 0);

    static bool DescrambleFile(const QByteArray& inputBuffer, QByteArray& outputBuffer);
    static bool ScrambleFile(const QByteArray& inputBuffer, QByteArray& outputBuffer);
};

#endif // TORCHLIGHT2STASHCONVERTER_H
