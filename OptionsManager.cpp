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
    mCurrentOptionsKey = "Default";
    mOptionsMap.insert("Default", OptionsSet());
}

bool OptionsManager::contains(const QString &name)
{
    return mOptionsMap.contains(name);
}

void OptionsManager::setCurrentKey(const QString& key)
{
    mCurrentOptionsKey = key;
}

QString OptionsManager::currentKey()
{
    return mCurrentOptionsKey;
}

void OptionsManager::setUseExtendedAscii(bool extendedAscii,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseExtendedAscii = extendedAscii;
}

bool OptionsManager::useExtendedAscii( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseExtendedAscii;
}

void OptionsManager::setUsePunctuation(int usePunct,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUsePunctuation = usePunct;
}

int OptionsManager::usePunctuation( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUsePunctuation;
}

void OptionsManager::setUseSymbols(int useSymbols,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseSymbols = useSymbols;
}

int OptionsManager::useSymbols( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseSymbols;
}

void OptionsManager::setUseDigits(int useDigits,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseDigits = useDigits;
}

int OptionsManager::useDigits( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseDigits;
}

void OptionsManager::setUseUpperAlpha(int useUpper,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseUpperAlpha = useUpper;
}

int OptionsManager::useUpperAlpha( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseUpperAlpha;
}

void OptionsManager::setUseLowerAlpha(int useLower,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseLowerAlpha = useLower;
}

int OptionsManager::useLowerAlpha( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseLowerAlpha;
}

void OptionsManager::setCharsToExclude(const QString& chars,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mCharsToExclude = chars;
}

QString OptionsManager::charsToExclude( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mCharsToExclude;
}

void OptionsManager::setPasswordLength(int length,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mPasswordLength = length;
}

int OptionsManager::passwordLength( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mPasswordLength;
}

void OptionsManager::setCopyToClipboard(bool copy,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mCopyToClipboard = copy;
}

bool OptionsManager::copyToClipboard( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mCopyToClipboard;
}

void OptionsManager::writeToJSON(QJsonObject& jsonObject) const
{
    // We may be using a filled object so we clear it.
    for (auto iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)
        jsonObject.erase(iter);

    // The index of the currently active set of options
    jsonObject["currentIndex"] = mCurrentOptionsKey;

    // Array of option sets
    QJsonArray jsonArray;
    for(auto iter = mOptionsMap.constBegin(); iter != mOptionsMap.constEnd(); ++iter)
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
            OptionsSet optRec;
            QJsonObject obj = optArray[idx].toObject();
            optRec.readFromJSON(obj);
            tempList.insert(optRec.mName, optRec);
        }
    }

    mCurrentOptionsKey = curIdx;
    mOptionsMap = tempList;
}

void OptionsManager::setName(const QString &name,  QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mName = name;
}

QString OptionsManager::name( QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mName;
}

OptionsManager::OptionsSet::OptionsSet()
{
    setToDefault();
}

void OptionsManager::OptionsSet::setToDefault()
{
    mName = "Default";
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

void OptionsManager::OptionsSet::writeToJSON(QJsonObject &jsonObj) const
{
    jsonObj["label"] = mName;
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

void OptionsManager::OptionsSet::readFromJSON(const QJsonObject &jsonObj)
{
    mName = jsonObj["label"].toString();
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
