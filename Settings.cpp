/*
 * Copyright © 2019 Tim Allman
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    return static_cast<int>(mSettings.value("UsePunctuation", CharacterPool::REQUEST).toInt());
}

void Settings::setUseSymbols(int useSymbols)
{
    mSettings.setValue("UseSymbols", useSymbols);
}

int Settings::useSymbols() const
{
    return static_cast<int>(mSettings.value("UseSymbols", CharacterPool::REQUEST).toInt());
}

void Settings::setUseDigits(int useDigits)
{
    mSettings.setValue("UseDigits", useDigits);
}

int Settings::useDigits() const
{
    return static_cast<int>(mSettings.value("UseDigits", CharacterPool::REQUEST).toInt());
}

void Settings::setUseUpperAlpha(int useUpper)
{
    mSettings.setValue("UseUpperAlpha", useUpper);
}

int Settings::useUpperAlpha() const
{
    return static_cast<int>(mSettings.value("UseUpperAlpha", CharacterPool::REQUEST).toInt());
}

void Settings::setUseLowerAlpha(int useLower)
{
    mSettings.setValue("UseLowerAlpha", useLower);
}

int Settings::useLowerAlpha() const
{
    return static_cast<int>(mSettings.value("UseLowerAlpha", CharacterPool::REQUEST).toInt());
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

QString Settings::charactersToExclude() const
{
    return mSettings.value("CharactersToExclude", "").toString();
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

