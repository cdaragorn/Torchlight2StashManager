#include "optionkeys.h"

const QString OptionKeys::Torchlight2SharedStashFile = "Torchlight2SharedStashFile";
const QString OptionKeys::StashManagerFolder = "StashManagerFolder";
const QString OptionKeys::MaxNumberOfItemsPerStashTab = "MaxNumberOfItemsPerStashTab";

OptionKeys::OptionKeys(QObject* parent)
    : QObject(parent)
{
}
