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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

#include "CharacterPool.h"

/**
 * This class provides a restricted and convenient front end to QSettings.
 * It is implemented as a singleton although this probably isn't necessary
 * because all instances of @c QSettings are portals to the same underlying
 * instance.
 */
class Settings
{
public:
    /**
     * The only way of getting an instance.
     * @return Reference to the instance.
     */
    static Settings& instance()
    {
        /** Pointer to the one instance of this class. */
        static Settings* instance;

        /** Create an instance if necessary */
        if (instance == nullptr)
            instance = new Settings;
        return *instance;
    }

public:
    ///@{
    /** Getter or setter for a settings value. Name is self explanatory. */
    void setUseExtendedAscii(bool extendedAscii);
    bool useExtendedAscii() const;

    void setUsePunctuation(int usePunct);
    int usePunctuation() const;

    void setUseSymbols(int useSymbols);
    int useSymbols() const;

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
    ///@}s

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
    /** Default private constructor. */
    Settings() = default;

    /** Destructor. */
    ~Settings() = default;

    /** The QSettings instance that does all the work. */
    QSettings mSettings;
};

#endif // SETTINGS_H
