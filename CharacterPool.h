/*
 * Charset.h
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

#ifndef CHARACTERPOOL_H
#define CHARACTERPOOL_H

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

#include <exception>

#include <QString>

/**
 * @brief Creates the character set from which passwords are made.
 *
 * This class manages the creation of the character set from which
 * passwords will be derived. It is constructed with information and options
 * selected by the user. @sa MainWindow
 */
class CharacterPool
{
public:
    enum
    {
        EXCLUDE = Qt::Unchecked,         ///< Do not use these characters.
        REQUEST = Qt::PartiallyChecked,  ///< These characters are allowed.
        REQUIRE = Qt::Checked            ///< These characters are required.
    };

    /**
     * Charset constructor.
     * @param useExtendedAscii ///< Use the full ISO-8859-1 code page if @c true. ASCII otherwise.
     * @param excludeChars     ///< Exclude characters set by user if @c true.
     * @param excludedChars    ///< The characters set by user to exclude if @c true.
     * @param usePunctuation   ///< Use punctuation, symbols etc. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE
     * @param useDigits        ///< Use digits. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
     * @param useUpperAlpha    ///< Use upper case letters. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
     * @param useLowerAlpha    ///< Use lower case letters. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
     */
    explicit CharacterPool(bool useExtendedAscii, bool excludeChars, const QString& excludedChars,
                     int usePunctuation, int useDigits, int useUpperAlpha,
                     int useLowerAlpha);

    /**
     * Deleted copy constructor.
     */
    CharacterPool(const CharacterPool&) = delete;

    /**
     * Deleted move constructor.
     */
    CharacterPool(const CharacterPool&&) = delete;

    /**
     * Deleted copy assignment operator.
     */
    CharacterPool& operator=(const CharacterPool&) = delete;

    /**
     * Deleted move assignment operator.
     */
    CharacterPool& operator=(const CharacterPool&&) = delete;

    /**
     * Get the list of punctuation and symbol characters.
     * @return The list of punctuation and symbol characters.
     */
    QString punctChars() const;

    /**
     * Get the list of digit characters.
     * @return The list of digit characters.
     */
    QString digitChars() const;

    /**
     * Get the list of upper case characters.
     * @return The list of upper case characters.
     */
    QString upperAlphaChars() const;

    /**
     * Get the list of lower case characters.
     * @return The list of lower case characters.
     */
    QString lowerAlphaChars() const;

    /**
     * Get the string of all characters to be used in password generation.
     * @return The string of all characters.
     */
    QString allChars() const;

private:
    /**
     * Creates the character set returned by allChars().
     */
    void makeCharacterSet();

    bool mUseExtendedAscii;  ///< Use the full ISO-8859-1 code page if @c true. ASCII otherwise.
    bool mExcludeChars;      ///< Exclude characters set by user if @c true.
    QString mExcludedChars;  ///< The characters set by user to exclude if @c true.
    int mUsePunctuation;     ///< Use punctuation, symbols etc. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
    int mUseDigits;          ///< Use digits. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
    int mUseUpperAlpha;      ///< Use upper case letters. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.
    int mUseLowerAlpha;      ///< Use lower case letters. Can be @c EXCLUDE, @c REQUEST or @c REQUIRE.

    QString mAllChars;          ///< The full character set to be used.
    QString mPunctChars;        ///< The punctuation character set to be used.
    QString mDigitChars;        ///< The digit character set to be used.
    QString mUpperAlphaChars;   ///< The upper case character set to be used.
    QString mLowerAlphaChars;   ///< The lower case character set to be used.
};

#endif // CHARSET_H
