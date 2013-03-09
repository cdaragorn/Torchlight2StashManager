#include "torchlight2stashconverter.h"

#include <QFile>
#include <fstream>
#include <cstring>
using namespace std;

Torchlight2StashConverter::Torchlight2StashConverter(QObject* parent)
    : QObject(parent)
{
}

/*
*   Torchlight 2 sharedstash v65 checksum calculator proof-of-concept by Shatter
*/
static unsigned int CalculateChecksum(const QByteArray& buffer, qint64 offset, qint64 length)
{
    quint32 checksum = 0x14d3; // 14d3 seed

    for (qint64 i = 0; i < length; ++i)
    {
        checksum += (checksum << 0x5);
        checksum += (quint8)buffer.constData()[offset + i];
//        checksum &= 0xFFFFFFFF; // (mask in case an int is 64bits somehow)
    }
    return checksum;
}

static void Descramble(const QByteArray& inputBuffer, qint64 inputOffset, qint64 length, QByteArray& outputBuffer, qint64 outputOffset)
{
    for (int i = 0; i < length; ++i)
    {
        // get least significant nybble
        quint8 leastSignificantNybble = (inputBuffer.constData()[inputOffset + i] & 0xF0) >> 4;
        // get most significant nybble
        quint8 mostSignificantNybble = inputBuffer.constData()[(inputOffset + (length - 1)) - i] & 0x0F;
        // exceptions: descrambled bytes of exactly 0x00 or 0xFF stay that way and do not get inverted!
        if (!(((leastSignificantNybble == 0) && (mostSignificantNybble == 0)) ||
                ((leastSignificantNybble == 0xF) && (mostSignificantNybble == 0xF))))
        {
            leastSignificantNybble ^= 0xF;
            mostSignificantNybble ^= 0xF;
        }
        outputBuffer.data()[outputOffset + i] = (mostSignificantNybble << 4) | leastSignificantNybble;
    }
}

static void Scramble(const QByteArray& inputBuffer, qint64 inputOffset, qint64 length, QByteArray& outputBuffer, qint64 outputOffset)
{
    for (qint64 i = 0; i < length; ++i)
    {
        quint8 mostSignificantNybble = inputBuffer.constData()[inputOffset + i] & 0x0F;
        quint8 leastSignificantNybble = (inputBuffer.constData()[inputOffset + i] & 0xF0) >> 4;

        if (!((mostSignificantNybble == 0 && leastSignificantNybble == 0) ||
              (mostSignificantNybble == 0xF && leastSignificantNybble == 0xF)))
        {
            leastSignificantNybble ^= 0xF;
            mostSignificantNybble ^= 0xF;
        }

        outputBuffer.data()[outputOffset + i] = (outputBuffer.data()[outputOffset + i] & 0x0F) | (mostSignificantNybble << 4);

//        outputBuffer.data()[outputBuffer + (length - 1) - i] = (qint8)((outputBuffer.data()[outputOffset + (length - 1) - i] & 0xF0) | leastSignificantNybble);

    }
}

bool Torchlight2StashConverter::DescrambleFile(QString inputFilePath, QString outputFilePath)
{
    bool result = false;

    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    if (inputFile.open(QIODevice::ReadOnly) && outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qint64 inputFileLength = inputFile.size();

        QByteArray inputBuffer = inputFile.readAll();
        inputFile.close();

        if (inputFileLength > BASE_HEADER_SIZE)
        {
            // offset 4 is almost always 0x01, except on corrupted character and stash files
            if ((quint8)inputBuffer[4] == 0x01)
            {
                quint32 version = reinterpret_cast<quint32>((char*)inputBuffer.data()[0]);

                bool isScrambled = true;
                bool isKnownSaveFileVersion = true;

                qint64 inputOffset = 0;
                qint64 outputFileLength = 0;

                switch(version)
                {
                    case 0x38:
                    {
                        cerr << "Beta version file, untested, your mileage may vary\n";
                        // for beta version we don't need to actually unscramble anything, just lop off the footer
                        // filesize of output will be the size of input minus four
                        outputFileLength = inputFileLength - FOOTER_SIZE;
                        inputOffset = BASE_HEADER_SIZE;
                        isScrambled = false;
                        break;
                    }
                    case 0x40:
                    {
                        cerr << "Release version 1.9 file, scrambled, no checksum\n";
                        // filesize of output will be the size of input minus four
                        outputFileLength = inputFileLength - FOOTER_SIZE;
                        inputOffset = BASE_HEADER_SIZE;
                        break;
                    }
                    case 0x41:
                    case 0x42:
                    {
                        cerr << "Patched version 1.10, 1.11 or 1.12 file, scrambled, with checksum\n";
                        // filesize of output will be the size of input minus eight
                        outputFileLength = inputFileLength - (CRC_SIZE + FOOTER_SIZE);
                        inputOffset = BASE_HEADER_SIZE + CRC_SIZE;
                        break;
                    }

                    default:
                    {
                        cerr << "Unknown savefile version!\n";
                        isKnownSaveFileVersion = false;
                        break;
                    }
                }

                if (isKnownSaveFileVersion)
                {
                    qint64 outputOffset = BASE_HEADER_SIZE;
                    qint64 dataLength = inputFileLength - (inputOffset + FOOTER_SIZE);

                    QByteArray outputBuffer(outputFileLength, 0);

                    if (version >= 0x40)
                    {
                        for (int i = 0; i < BASE_HEADER_SIZE; ++i)
                        {
                            outputBuffer[i] = inputBuffer[i];
                        }

                        // descramble the data and place it in the correct place in the output buffer, after the header
                        Descramble(inputBuffer, inputOffset, dataLength, outputBuffer, outputOffset);
                    }
                    else
                    {
                        // just copy the data directly to the output buffer
                        outputBuffer.resize(dataLength + BASE_HEADER_SIZE);
                        outputBuffer.replace(0, dataLength + BASE_HEADER_SIZE, inputBuffer);
//                        memcpy(outputBuffer, inputBuffer, dataLength + BASE_HEADER_SIZE);
                    }

                    if (version >= 0x41)
                    {
                        qint32 claimedCRC = reinterpret_cast<qint32*>(&(inputBuffer.data()[BASE_HEADER_SIZE]))[0];

                        qint32 calculatedCRC = CalculateChecksum(inputBuffer, inputOffset, dataLength);

                        cerr << "CRC of the data block: Header: " << claimedCRC << " Actual: " << calculatedCRC << endl;
                    }

                    qint32 lengthClaimed = reinterpret_cast<qint32>((char*)inputBuffer.data()[inputFileLength - 4]);

                    outputFile.write(outputBuffer);
                    outputFile.close();
//                    outputFile.write((char*)outputBuffer, outputFileLength);
//                    outputFile.close();

                    result = true;
                }
            }
        }



    }

    return result;
}

bool Torchlight2StashConverter::ScrambleFile(QString inputFilePath, QString outputFilePath)
{
    bool result = false;

    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    if (inputFile.open(QIODevice::ReadOnly) && outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {

    }


    return result;
}
