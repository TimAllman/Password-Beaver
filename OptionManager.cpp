#include "OptionManager.h"

#include <QJsonArray>

OptionsManager::OptionsManager()
{

}

OptionsManager::OptionsManager(const Settings& settings)
{

}

void OptionsManager::setUseExtendedAscii(bool extendedAscii, int index)
{
    mOptionsList[index].mUseExtendedAscii = extendedAscii;
}

bool OptionsManager::useExtendedAscii(int index) const
{
    return mOptionsList[index].mUseExtendedAscii;
}

void OptionsManager::setUsePunctuation(int usePunct, int index)
{
    mOptionsList[index].mUsePunctuation = usePunct;
}

int OptionsManager::usePunctuation(int index) const
{
    return mOptionsList[index].mUsePunctuation;
}

void OptionsManager::setUseSymbols(int useSymbols, int index)
{
    mOptionsList[index].mUseSymbols = useSymbols;
}

int OptionsManager::useSymbols(int index) const
{
    return mOptionsList[index].mUseSymbols;
}

void OptionsManager::setUseDigits(int useDigits, int index)
{
    mOptionsList[index].mUseDigits = useDigits;
}

int OptionsManager::useDigits(int index) const
{
    return mOptionsList[index].mUseDigits;
}

void OptionsManager::setUseUpperAlpha(int useUpper, int index)
{
    mOptionsList[index].mUseUpperAlpha = useUpper;
}

int OptionsManager::useUpperAlpha(int index) const
{
    return mOptionsList[index].mUseUpperAlpha;
}

void OptionsManager::setUseLowerAlpha(int useLower, int index)
{
    mOptionsList[index].mUseLowerAlpha = useLower;
}

int OptionsManager::useLowerAlpha(int index) const
{
return mOptionsList[index].mUseLowerAlpha;
}

void OptionsManager::setExcludeCharacters(bool exclude, int index)
{
    mOptionsList[index].mExcludeCharacters = exclude;
}

bool OptionsManager::excludeCharacters(int index) const
{
return mOptionsList[index].mExcludeCharacters;
}

void OptionsManager::setCharactersToExclude(const QString& chars, int index)
{
    mOptionsList[index].mCharactersToExclude = chars;
}

QString OptionsManager::charactersToExclude(int index) const
{
    return mOptionsList[index].mCharactersToExclude;
}

void OptionsManager::setPasswordLength(int length, int index)
{
    mOptionsList[index].mPasswordLength = length;
}

int OptionsManager::passwordLength(int index) const
{
    return mOptionsList[index].mPasswordLength;
}

void OptionsManager::setCopyToClipboard(bool copy, int index)
{
    mOptionsList[index].mCopyToClipboard = copy;
}

bool OptionsManager::copyToClipboard(int index) const
{
    return mOptionsList[index].mCopyToClipboard;
}

void OptionsManager::writeToJSON(QJsonArray &jsonArray)
{
    // We can't clear a QJsonArray so we do it this way.
    jsonArray = QJsonArray();

    jsonArray.append(QJsonValue(mCurrentOptionsIndex));
    for(auto iter = mOptionsList.begin(); iter != mOptionsList.end(); ++iter)
    {
        QJsonObject jsonObj;
        iter->writeToJSON(jsonObj);
        jsonArray.append(jsonObj);
    }
}

void OptionsManager::OptionsRec::writeToJSON(QJsonObject &jsonObj)
{
    jsonObj["label"] = mLabel;
    jsonObj["useExtendedAscii"] = mUseExtendedAscii;
    jsonObj["usePunctuation"] = mUsePunctuation;
    jsonObj["useSymbols"] = mUseSymbols;
    jsonObj["useDigits"] = mUseDigits;
    jsonObj["useUpperAlpha"] = mUseUpperAlpha;
    jsonObj["useLowerAlpha"] = mUseLowerAlpha;
    jsonObj["excludeCharacters"] = mExcludeCharacters;
    jsonObj["charactersToExclude"] = mCharactersToExclude;
    jsonObj["passwordLength"] = mPasswordLength;
    jsonObj["copyToClipboard"] = mCopyToClipboard;
}
