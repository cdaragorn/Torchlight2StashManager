#include "effectstable.h"

#include <QSqlQuery>
#include <QVariant>

const QString EffectsTable::EffectsTableName = "EffectsTable";
const QString EffectsTable::EffectId = "EffectId";
const QString EffectsTable::EffectNumber = "EffectNumber";
const QString EffectsTable::EffectRawName = "Effect";
const QString EffectsTable::Name = "Name";
const QString EffectsTable::GoodDescription = "GoodDescription";
const QString EffectsTable::GoodDescriptionOverTime = "GoodDescriptionOverTime";
const QString EffectsTable::BadDescription = "BadDescription";
const QString EffectsTable::BadDescriptionOverTime = "BadDescriptionOverTime";
const QString EffectsTable::DisplayPrecision = "DisplayPrecision";
const QString EffectsTable::DisplayPrecisionMaxVal = "DisplayPrecisionMaxVal";

EffectsTable::EffectsTable(QString databaseName)
{
    DatabaseName(databaseName);
}

Effect EffectsTable::ParseRecord(QSqlRecord record)
{
    Effect result;

    if (!record.isEmpty())
    {
        QSqlField effectId = record.field(EffectId);
        QSqlField effectNumber = record.field(EffectNumber);
        QSqlField effectRawName = record.field(EffectRawName);
        QSqlField name = record.field(Name);
        QSqlField goodDescription = record.field(GoodDescription);
        QSqlField goodDescriptionOverTime = record.field(GoodDescriptionOverTime);
        QSqlField badDescription = record.field(BadDescription);
        QSqlField badDescriptionOverTime = record.field(BadDescriptionOverTime);
        QSqlField displayPrecision = record.field(DisplayPrecision);
        QSqlField displayPrecisionMaxValue = record.field(DisplayPrecisionMaxVal);

        result.effectId = effectId.value().toLongLong();
        result.effectNumber = effectNumber.value().toInt();
        result.rawName = effectRawName.value().toString();
        result.name = name.value().toString();
        result.goodDescription = goodDescription.value().toString();
        result.goodDescriptionOverTime = goodDescriptionOverTime.value().toString();
        result.badDescription = badDescription.value().toString();
        result.badDescriptionOverTime = badDescriptionOverTime.value().toString();
        result.displayPrecision = displayPrecision.value().toInt();
        result.displayPrecisionMaxVal = displayPrecisionMaxValue.value().toInt();
    }

    return result;
}


Effect EffectsTable::GetEffect(qint32 effectNumber)
{
    Effect result;

    QSqlDatabase db = GetDatabase();

    if (db.open())
    {
        QSqlQuery query(db);

        if (query.prepare("SELECT * FROM " + EffectsTableName + " WHERE " + EffectNumber + " = :effectNumber"))
        {
            query.bindValue(":effectNumber", effectNumber);

            if (query.exec() && query.next())
            {
                result = ParseRecord(query.record());
            }
            else
            {
                PrintSqlError(query.lastError());
            }
        }
        else
        {
            PrintSqlError(query.lastError());
        }

        db.close();
    }

    return result;
}
