#include "OptionsManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>

const QString OptionsManager::STR_OPTIONS = "Options";
const QString OptionsManager::STR_DEFAULT = "Default";
const QString OptionsManager::STR_NAME = "Name";
const QString OptionsManager::STR_CURRENT_KEY = "CurrentKey";
const QString OptionsManager::STR_CHARS_TO_EXCLUDE = "CharsToExclude";
const QString OptionsManager::STR_USE_EXTENDED_ASCII = "UseExtendedAscii";
const QString OptionsManager::STR_USE_PUNCTUATION = "UsePunctuation";
const QString OptionsManager::STR_USE_SYMBOLS = "UseSymbols";
const QString OptionsManager::STR_USE_DIGITS = "UseDigits";
const QString OptionsManager::STR_USE_LOWER_ALPHA = "UseLowerAlpha";
const QString OptionsManager::STR_USE_UPPER_ALPHA = "UseUpperAlpha";
const QString OptionsManager::STR_PASSWORD_LENGTH = "PasswordLength";
const QString OptionsManager::STR_COPY_TO_CLIPBOARD = "CopyToClipboard";

OptionsManager::OptionsManager()
{
    QSettings settings;
    QByteArray jsonString;

    setDefaults();

    if (settings.contains(STR_OPTIONS))
    {
        jsonString = settings.value(STR_OPTIONS).toByteArray();

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
    mCurrentOptionsKey = STR_DEFAULT;
    mOptionsMap.insert(STR_DEFAULT, OptionsSet());
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

void OptionsManager::setUseExtendedAscii(bool extendedAscii, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseExtendedAscii = extendedAscii;
}

bool OptionsManager::useExtendedAscii(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseExtendedAscii;
}

void OptionsManager::setUsePunctuation(int usePunct, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUsePunctuation = usePunct;
}

int OptionsManager::usePunctuation(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUsePunctuation;
}

void OptionsManager::setUseSymbols(int useSymbols, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseSymbols = useSymbols;
}

int OptionsManager::useSymbols(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseSymbols;
}

void OptionsManager::setUseDigits(int useDigits, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseDigits = useDigits;
}

int OptionsManager::useDigits(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseDigits;
}

void OptionsManager::setUseUpperAlpha(int useUpper, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseUpperAlpha = useUpper;
}

int OptionsManager::useUpperAlpha(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseUpperAlpha;
}

void OptionsManager::setUseLowerAlpha(int useLower, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mUseLowerAlpha = useLower;
}

int OptionsManager::useLowerAlpha(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mUseLowerAlpha;
}

void OptionsManager::setCharsToExclude(const QString& chars, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mCharsToExclude = chars;
}

QString OptionsManager::charsToExclude(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mCharsToExclude;
}

void OptionsManager::setPasswordLength(int length, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mPasswordLength = length;
}

int OptionsManager::passwordLength(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mPasswordLength;
}

void OptionsManager::setCopyToClipboard(bool copy, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mCopyToClipboard = copy;
}

bool OptionsManager::copyToClipboard(QString key) const
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

    // The key of the currently active set of options
    jsonObject[STR_CURRENT_KEY] = mCurrentOptionsKey;

    // Array of option sets
    QJsonArray jsonArray;
    for(auto iter = mOptionsMap.constBegin(); iter != mOptionsMap.constEnd(); ++iter)
    {
        QJsonObject jsonObj;
        iter->writeToJSON(jsonObj);
        jsonArray.append(jsonObj);
    }
    jsonObject[STR_OPTIONS] = jsonArray;
}

void OptionsManager::readFromJSON(const QJsonObject& jsonObject)
{
    QString curKey;
    if (jsonObject.contains(STR_CURRENT_KEY))
        curKey = jsonObject.value(STR_CURRENT_KEY).toString();
    else
    {
        //throw something;
    }

    ListType tempList;
    if (jsonObject.contains(STR_OPTIONS) && jsonObject[STR_OPTIONS].isArray())
    {
        QJsonArray optArray = jsonObject[STR_OPTIONS].toArray();
        for (int idx = 0; idx < optArray.size(); ++idx)
        {
            OptionsSet optRec;
            QJsonObject obj = optArray[idx].toObject();
            optRec.readFromJSON(obj);
            tempList.insert(optRec.mName, optRec);
            mCurrentOptionsKey = optRec.mName;
        }
    }

    mCurrentOptionsKey = curKey;
    mOptionsMap = tempList;
}

void OptionsManager::setName(const QString &name, QString key)
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    mOptionsMap[key].mName = name;
}

QString OptionsManager::name(QString key) const
{
    if (key.isEmpty())
        key = mCurrentOptionsKey;
    return mOptionsMap[key].mName;
}

QStringList OptionsManager::names()
{
    // This works because the keys are the same as the names in the OptionsSets
    return mOptionsMap.keys();
}

void OptionsManager::saveOptions(const QString& newName)
{
    OptionsSet newOpts = mOptionsMap[mCurrentOptionsKey];
    newOpts.mName = newName;
    mOptionsMap.insert(newName, newOpts);
    mCurrentOptionsKey = newName;
}

void OptionsManager::switchOptions(const QString &name)
{

}

OptionsManager::OptionsSet::OptionsSet()
{
    setToDefault();
}

void OptionsManager::OptionsSet::setToDefault()
{
    mName = OptionsManager::STR_DEFAULT;
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
    jsonObj[STR_NAME] = mName;
    jsonObj[STR_CHARS_TO_EXCLUDE] = mCharsToExclude;
    jsonObj[STR_USE_EXTENDED_ASCII] = mUseExtendedAscii;
    jsonObj[STR_USE_PUNCTUATION] = mUsePunctuation;
    jsonObj[STR_USE_SYMBOLS] = mUseSymbols;
    jsonObj[STR_USE_DIGITS] = mUseDigits;
    jsonObj[STR_USE_UPPER_ALPHA] = mUseUpperAlpha;
    jsonObj[STR_USE_LOWER_ALPHA] = mUseLowerAlpha;
    jsonObj[STR_PASSWORD_LENGTH] = mPasswordLength;
    jsonObj[STR_COPY_TO_CLIPBOARD] = mCopyToClipboard;
}

void OptionsManager::OptionsSet::readFromJSON(const QJsonObject &jsonObj)
{
    mName = jsonObj[STR_NAME].toString();
    mUseExtendedAscii = jsonObj[STR_USE_EXTENDED_ASCII].toBool();
    mUsePunctuation = jsonObj[STR_USE_PUNCTUATION].toInt();
    mUseSymbols = jsonObj[STR_USE_SYMBOLS].toInt();
    mUseDigits = jsonObj[STR_USE_DIGITS].toInt();
    mUseUpperAlpha = jsonObj[STR_USE_UPPER_ALPHA].toInt();
    mUseLowerAlpha = jsonObj[STR_USE_LOWER_ALPHA].toInt();
    mCharsToExclude = jsonObj[STR_CHARS_TO_EXCLUDE].toString();
    mPasswordLength = jsonObj[STR_PASSWORD_LENGTH].toInt();
    mCopyToClipboard = jsonObj[STR_COPY_TO_CLIPBOARD].toBool();
}
