/*
 * Settings.h
 *
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "Charset.h"

class Settings
{
public:
    static Settings& instance()
    {
        static Settings instance_; // Guaranteed to be instantiated on first use
        return instance_;          // and destroyed when out of scope.
    }

public:
    void setUseExtendedAscii(bool extendedAscii);
    bool useExtendedAscii() const;

    void setUsePunctuation(int usePunct);
    int usePunctuation() const;

    void setUseDigits(int useDigits);
    int useDigits() const;

    void setUseUpperAlpha(int useUpper);
    int useUpperAlpha() const;

    void setUseLowerAlpha(int useLower);
    int useLowerAlpha() const;

    void setExcludeCharacters(bool exclude);
    bool excludeCharacters() const;

    void setCharactersToExclude(const QString& chars);
    QString charactersToExclude() const;

    void setPasswordLength(int length);
    int passwordLength() const;

    void setCopyToClipboard(bool copy);
    bool copyToClipboard() const;

    void setWindowGeometry(const QByteArray& geometry);
    QByteArray windowGeometry() const;

    /**
     * Do not allow copy construction
     */
    Settings(const Settings&) = delete;

    /**
     * Do not allow move construction
     */
    Settings(const Settings&&) = delete;

    /**
     * Do not allow copy assignment
     */
    void operator=(const Settings&) = delete;

    /**
     * Do not allow move assignment
     */
    void operator=(const Settings&&) = delete;

private:
    Settings() = default;

    QSettings mSettings;
};

#endif // SETTINGS_H
