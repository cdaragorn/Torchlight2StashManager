#ifndef EFFECTSTABLE_H
#define EFFECTSTABLE_H

#include <sqlitetable.h>
#include <QSqlRecord>


struct Effect
{
    Effect()
    {
        Clear();
    }

    void Clear()
    {
        effectId = 0;
        effectNumber = 0;
        rawName = "";
        name = "";
        goodDescription = "";
        goodDescriptionOverTime = "";
        badDescription = "";
        badDescriptionOverTime = "";
        displayPrecision = 0;
        displayPrecisionMaxVal = 0;
    }

    qint64 effectId;
    qint32 effectNumber;
    QString rawName;
    QString name;
    QString goodDescription;
    QString goodDescriptionOverTime;
    QString badDescription;
    QString badDescriptionOverTime;
    qint32 displayPrecision;
    qint32 displayPrecisionMaxVal;
};

class EffectsTable : public SqliteTable
{
public:
    EffectsTable(QString databaseName);

    Effect GetEffect(qint32 effectNumber);

private:

    Effect ParseRecord(QSqlRecord record);

    static const QString EffectsTableName;
    static const QString EffectId;
    static const QString EffectNumber;
    static const QString EffectRawName;
    static const QString Name;
    static const QString GoodDescription;
    static const QString GoodDescriptionOverTime;
    static const QString BadDescription;
    static const QString BadDescriptionOverTime;
    static const QString DisplayPrecision;
    static const QString DisplayPrecisionMaxVal;
};

#endif // EFFECTSTABLE_H
