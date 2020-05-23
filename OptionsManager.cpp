#include "OptionsManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>

OptionsManager::OptionsManager()
{
    QSettings settings;
    QByteArray jsonString;

    setDefaults();

    if (settings.contains("Options"))
    {
        jsonString = settings.value("Options").toByteArray();

        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString, &jsonErr);

        QJsonObject jsonObj = jsonDoc.object();
        readFromJSON(jsonObj);
    }
}

OptionsManager &OptionsManager::instance()
{
    /** Pointer to the one instance of this class. */
    static OptionsManager* instance;

    /** Create an instance if necessary */
    if (instance == nullptr)
        instance = new OptionsManager;
    return *instance;
}

void OptionsManager::setDefaults()
{
    mCurrentOptionsIndex = 0;
    mOptionsList.append(OptionsRec());
}

void OptionsManager::setCurrentIndex(int index)
{
    mCurrentOptionsIndex = index;
}

int OptionsManager::currentIndex()
{
    return mCurrentOptionsIndex;
}

void OptionsManager::setUseExtendedAscii(bool extendedAscii, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUseExtendedAscii = extendedAscii;
}

bool OptionsManager::useExtendedAscii(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUseExtendedAscii;
}

void OptionsManager::setUsePunctuation(int usePunct, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUsePunctuation = usePunct;
}

int OptionsManager::usePunctuation(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUsePunctuation;
}

void OptionsManager::setUseSymbols(int useSymbols, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUseSymbols = useSymbols;
}

int OptionsManager::useSymbols(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUseSymbols;
}

void OptionsManager::setUseDigits(int useDigits, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUseDigits = useDigits;
}

int OptionsManager::useDigits(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUseDigits;
}

void OptionsManager::setUseUpperAlpha(int useUpper, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUseUpperAlpha = useUpper;
}

int OptionsManager::useUpperAlpha(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUseUpperAlpha;
}

void OptionsManager::setUseLowerAlpha(int useLower, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mUseLowerAlpha = useLower;
}

int OptionsManager::useLowerAlpha(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mUseLowerAlpha;
}

void OptionsManager::setCharsToExclude(const QString& chars, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mCharsToExclude = chars;
}

QString OptionsManager::charsToExclude(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mCharsToExclude;
}

void OptionsManager::setPasswordLength(int length, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mPasswordLength = length;
}

int OptionsManager::passwordLength(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mPasswordLength;
}

void OptionsManager::setCopyToClipboard(bool copy, int index)
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    mOptionsList[index].mCopyToClipboard = copy;
}

bool OptionsManager::copyToClipboard(int index) const
{
    if (index == -1)
        index = mCurrentOptionsIndex;
    return mOptionsList[index].mCopyToClipboard;
}

void OptionsManager::writeToJSON(QJsonObject& jsonObject) const
{
    // We may be using a filled object so we clear it.
    for (auto iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)
        jsonObject.erase(iter);

    // The index of the currently active set of options
    jsonObject["currentIndex"] = mCurrentOptionsIndex;

    // Array of option sets
    QJsonArray jsonArray;
    for(auto iter = mOptionsList.constBegin(); iter != mOptionsList.constEnd(); ++iter)
    {
        QJsonObject jsonObj;
        iter->writeToJSON(jsonObj);
        jsonArray.append(jsonObj);
    }
    jsonObject["Options"] = jsonArray;
}

void OptionsManager::readFromJSON(const QJsonObject& jsonObject)
{
    int curIdx = 0;
    if (jsonObject.contains("currentIndex"))
        curIdx = jsonObject.value("currentIndex").toInt();
    else
    {
        //throw something;
    }

    ListType tempList;
    if (jsonObject.contains("Options") && jsonObject["Options"].isArray())
    {
        QJsonArray optArray = jsonObject["Options"].toArray();
        for (int idx = 0; idx < optArray.size(); ++idx)
        {
            OptionsRec optRec;
            QJsonObject obj = optArray[idx].toObject();
            optRec.readFromJSON(obj);
            tempList.append(optRec);
        }
    }

    mCurrentOptionsIndex = curIdx;
    mOptionsList = tempList;
}

OptionsManager::OptionsRec::OptionsRec()
{
    setToDefault();
}

void OptionsManager::OptionsRec::setToDefault()
{
    mLabel = "default";
    mUseExtendedAscii = false;
    mUsePunctuation = 2;
    mUseSymbols = 2;
    mUseDigits = 2;
    mUseUpperAlpha = 2;
    mUseLowerAlpha = 2;
    mCharsToExclude = "";
    mPasswordLength = 16;
    mCopyToClipboard = true;
}

void OptionsManager::OptionsRec::writeToJSON(QJsonObject &jsonObj) const
{
    jsonObj["label"] = mLabel;
    jsonObj["charsToExclude"] = mCharsToExclude;
    jsonObj["useExtendedAscii"] = mUseExtendedAscii;
    jsonObj["usePunctuation"] = mUsePunctuation;
    jsonObj["useSymbols"] = mUseSymbols;
    jsonObj["useDigits"] = mUseDigits;
    jsonObj["useUpperAlpha"] = mUseUpperAlpha;
    jsonObj["useLowerAlpha"] = mUseLowerAlpha;
    jsonObj["passwordLength"] = mPasswordLength;
    jsonObj["copyToClipboard"] = mCopyToClipboard;
}

void OptionsManager::OptionsRec::readFromJSON(const QJsonObject &jsonObj)
{
    mLabel = jsonObj["label"].toString();
    mUseExtendedAscii = jsonObj["useExtendedAscii"].toBool();
    mUsePunctuation = jsonObj["usePunctuation"].toInt();
    mUseSymbols = jsonObj["useSymbols"].toInt();
    mUseDigits = jsonObj["useDigits"].toInt();
    mUseUpperAlpha = jsonObj["useUpperAlpha"].toInt();
    mUseLowerAlpha = jsonObj["useLowerAlpha"].toInt();
    mCharsToExclude = jsonObj["charsToExclude"].toString();
    mPasswordLength = jsonObj["passwordLength"].toInt();
    mCopyToClipboard = jsonObj["copyToClipboard"].toBool();
}
