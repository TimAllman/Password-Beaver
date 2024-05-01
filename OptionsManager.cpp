#include "OptionsManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>

const QString OptionsManager::STR_OPTIONS = "Options";
const QString OptionsManager::STR_DEFAULT_NAME = "Default";
const QString OptionsManager::STR_NAME = "Name";
const QString OptionsManager::STR_ACTIVE_KEY = "ActiveKey";
const QString OptionsManager::STR_CHARS_TO_EXCLUDE = "CharsToExclude";
const QString OptionsManager::STR_DEF_CHARS_TO_EXCLUDE = "O0|l1";
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
    setToDefault();

    QSettings settings;
    if (settings.contains(STR_OPTIONS))
    {
        QByteArray jsonString = settings.value(STR_OPTIONS).toByteArray();

        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString, &jsonErr);

        QJsonObject jsonObj = jsonDoc.object();
        readFromJSON(jsonObj);
    }

    setActive(mActiveOptionsKey);
}

OptionsManager& OptionsManager::instance()
{
    /** The one instance of this class. */
    static OptionsManager instance;

    return instance;
}

void OptionsManager::setToDefault()
{
    mActiveOptionsKey = STR_DEFAULT_NAME;
    mOptionsMap.insert(STR_DEFAULT_NAME, OptionsSet());
    mActiveOptions = mOptionsMap.value(mActiveOptionsKey);
}

bool OptionsManager::contains(const QString &name)
{
    return mOptionsMap.contains(name);
}

void OptionsManager::setActiveKey(const QString& key)
{
    mActiveOptionsKey = key;
}

QString OptionsManager::activeKey()
{
    return mActiveOptionsKey;
}

void OptionsManager::setUseExtendedAscii(bool useExtendedAscii)
{
    mActiveOptions.mUseExtendedAscii = useExtendedAscii;
}

bool OptionsManager::useExtendedAscii() const
{
    return mActiveOptions.mUseExtendedAscii;
}

void OptionsManager::setUsePunctuation(int usePunct)
{
    mActiveOptions.mUsePunctuation = usePunct;
}

int OptionsManager::usePunctuation() const
{
    return mActiveOptions.mUsePunctuation;
}

void OptionsManager::setUseSymbols(int useSymbols)
{
    mActiveOptions.mUseSymbols = useSymbols;
}

int OptionsManager::useSymbols() const
{
    return mActiveOptions.mUseSymbols;
}

void OptionsManager::setUseDigits(int useDigits)
{
    mActiveOptions.mUseDigits = useDigits;
}

int OptionsManager::useDigits() const
{
    return mActiveOptions.mUseDigits;
}

void OptionsManager::setUseUpperAlpha(int useUpper)
{
    mActiveOptions.mUseUpperAlpha = useUpper;
}

int OptionsManager::useUpperAlpha() const
{
    return mActiveOptions.mUseUpperAlpha;
}

void OptionsManager::setUseLowerAlpha(int useLower)
{
    mActiveOptions.mUseLowerAlpha = useLower;
}

int OptionsManager::useLowerAlpha() const
{
    return mActiveOptions.mUseLowerAlpha;
}

void OptionsManager::setCharsToExclude(const QString& chars)
{
    mActiveOptions.mCharsToExclude = chars;
}

QString OptionsManager::charsToExclude() const
{
    return mActiveOptions.mCharsToExclude;
}

void OptionsManager::setPasswordLength(int length)
{
    mActiveOptions.mPasswordLength = length;
}

int OptionsManager::passwordLength() const
{
    return mActiveOptions.mPasswordLength;
}

void OptionsManager::setCopyToClipboard(bool copy)
{
    mActiveOptions.mCopyToClipboard = copy;
}

bool OptionsManager::autoCopyToClipboard() const
{
    return mActiveOptions.mCopyToClipboard;
}

void OptionsManager::writeToJSON(QJsonObject& jsonObject) const
{
    // We may be using a filled object so we clear it.
    // The class QJsonObject may generate a warning about the trivial copy-constructor
    // in the presence of a non-trivial copy-assignment operator. Blame Qt.
    for (auto iter = jsonObject.begin(); iter != jsonObject.end(); ++iter)
        jsonObject.erase(iter);

    // The key of the currently active set of options
    jsonObject[STR_ACTIVE_KEY] = mActiveOptionsKey;

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
    if (jsonObject.contains(STR_ACTIVE_KEY))
        curKey = jsonObject.value(STR_ACTIVE_KEY).toString();
    else
    {
        // TODO Make this more robust.
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
            mActiveOptionsKey = optRec.mName;
        }
    }

    mActiveOptionsKey = curKey;
    mOptionsMap = tempList;
}

void OptionsManager::setName(const QString &name)
{
    mActiveOptions.mName = name;
}

QString OptionsManager::name() const
{
    return mActiveOptions.mName;
}

bool OptionsManager::isModified(const QString &name)
{
    // Considered modified if
    //   - the name is not a key in the map.
    //   - the name is in the map and mActiveOptions is not equal to mOptionsMap[name].
    if (!mOptionsMap.contains(name))
        return true;

    if (!mActiveOptions.comparePasswordOptions(mOptionsMap.value(name)))
        return true;

    return false;
}

QStringList OptionsManager::names()
{
    // This works because the keys are the same as the names in the OptionsSets
    return mOptionsMap.keys();
}

void OptionsManager::setActive(const QString& name)
{
    if (contains(name))
    {
        mActiveOptions = mOptionsMap.value(name);
        mActiveOptionsKey = name;
    }
}

void OptionsManager::saveOptions(const QString& newName)
{
    // never overwrite "Default"
    if (newName == STR_DEFAULT_NAME)
        return;

    // We want to copy the active opts buffer to the map.
    // If the key exists in the map we overwrite the current entry
    // If key does not exist we create a new entry. The [] operator does just what we want.
    mActiveOptions.mName = newName;
    mOptionsMap[newName] = mActiveOptions;
    mActiveOptionsKey = newName;
}

void OptionsManager::deleteOptions(const QString &name)
{
    // never delete "Default"
    if (name == STR_DEFAULT_NAME)
        return;

    auto iter = mOptionsMap.find(name);
    if (iter != mOptionsMap.end())
    {
        auto newIter = iter;
        if (iter == mOptionsMap.begin())
            ++newIter;
        else
            --newIter;

        mOptionsMap.erase(static_cast<ListType::const_iterator>(iter));

        setActive(newIter->mName);
    }
}

OptionsManager::OptionsSet::OptionsSet()
{
    setDefaultPwOptions();

    mName = OptionsManager::STR_DEFAULT_NAME;
    mCopyToClipboard = true;
}

void OptionsManager::OptionsSet::setDefaultPwOptions()
{
    mUseExtendedAscii = false;
    mUsePunctuation = 2;
    mUseSymbols = 2;
    mUseDigits = 2;
    mUseUpperAlpha = 2;
    mUseLowerAlpha = 2;
    mCharsToExclude = STR_DEF_CHARS_TO_EXCLUDE;
    mPasswordLength = 16;
}

void OptionsManager::OptionsSet::writeToJSON(QJsonObject& jsonObj) const
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

void OptionsManager::OptionsSet::readFromJSON(const QJsonObject& jsonObj)
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

bool OptionsManager::OptionsSet::comparePasswordOptions(const OptionsManager::OptionsSet& other) const
{
    // Compare all options that affect password creation.
    return ((mUseExtendedAscii == other.mUseExtendedAscii) &&
            (mUsePunctuation == other.mUsePunctuation) &&
            (mUseSymbols == other.mUseSymbols) &&
            (mUseDigits == other.mUseDigits) &&
            (mUseUpperAlpha == other.mUseUpperAlpha) &&
            (mUseLowerAlpha == other.mUseLowerAlpha) &&
            (mCharsToExclude == other.mCharsToExclude) &&
            (mPasswordLength == other.mPasswordLength));
}

bool operator==(const OptionsManager::OptionsSet& lhs, const OptionsManager::OptionsSet& rhs)
{
    // Compare two @c OptionsSet instances completely.
    return ((lhs.mName == rhs.mName) &&
            (lhs.mCopyToClipboard == rhs.mCopyToClipboard) &&
            lhs.comparePasswordOptions(rhs));
}

bool operator!=(const OptionsManager::OptionsSet &lhs, const OptionsManager::OptionsSet &rhs)
{
    return !(lhs == rhs);
}
