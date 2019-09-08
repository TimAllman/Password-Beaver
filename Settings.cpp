#include "Settings.h"

void Settings::setUseExtendedAscii(bool extendedAscii)
{
    mSettings.setValue("ExtendedAscii", extendedAscii);
}

bool Settings::useExtendedAscii() const
{
    return mSettings.value("ExtendedAscii", true).toBool();
}

void Settings::setUsePunctuation(int usePunct)
{
    mSettings.setValue("UsePunctuation", usePunct);
}

int Settings::usePunctuation() const
{
    return static_cast<int>(mSettings.value("UsePunctuation", Charset::REQUIRE).toInt());
}

void Settings::setUseDigits(int useDigits)
{
    mSettings.setValue("UseDigits", useDigits);
}

int Settings::useDigits() const
{
    return static_cast<int>(mSettings.value("UseDigits", Charset::REQUIRE).toInt());
}

void Settings::setUseUpperAlpha(int useUpper)
{
    mSettings.setValue("UseUpperAlpha", useUpper);
}

int Settings::useUpperAlpha() const
{
    return static_cast<int>(mSettings.value("UseUpperAlpha", Charset::REQUIRE).toInt());
}

void Settings::setUseLowerAlpha(int useLower)
{
    mSettings.setValue("UseLowerAlpha", useLower);
}

int Settings::useLowerAlpha() const
{
    return static_cast<int>(mSettings.value("UseLowerAlpha", Charset::REQUIRE).toInt());
}

void Settings::setExcludeCharacters(bool exclude)
{
    mSettings.setValue("ExcludeCharacters", exclude);
}

bool Settings::excludeCharacters() const
{
    return mSettings.value("ExcludeCharacters", false).toBool();
}

void Settings::setCharactersToExclude(const QString& chars)
{
    mSettings.setValue("CharactersToExclude", chars);
}

//void Settings::setStringsToExclude(const QStringList& strings)
//{
//    mSettings.setValue("StringsToExclude", strings);
//}

//QStringList Settings::stringsToExclude() const
//{
//    return mSettings.value("StringsToExclude").toStringList();
//}

QString Settings::charactersToExclude() const
{
    return mSettings.value("CharactersToExclude", "").toString();

//    QStringList strings = stringsToExclude();

//    if (!strings.isEmpty())
//        return strings.first();
//    else
//        return QString();
}

void Settings::setPasswordLength(int length)
{
    mSettings.setValue("PasswordLength", length);
}

int Settings::passwordLength() const
{
    return mSettings.value("PasswordLength", 16).toInt();
}

void Settings::setCopyToClipboard(bool copy)
{
    mSettings.setValue("CopyToClipboard", copy);
}

bool Settings::copyToClipboard() const
{
    return mSettings.value("CopyToClipboard", true).toBool();
}

void Settings::setWindowGeometry(const QByteArray& geometry)
{
    mSettings.setValue("WindowGeometry", geometry);
}

QByteArray Settings::windowGeometry() const
{
    return mSettings.value("WindowGeometry").toByteArray();
}
