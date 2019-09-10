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

#ifndef CHARSET_H
#define CHARSET_H

#include <exception>

#include <QString>

class Charset
{
public:
    static const int EXCLUDE = Qt::Unchecked;
    static const int REQUEST = Qt::PartiallyChecked;
    static const int REQUIRE = Qt::Checked;

//    enum int
//    {
//        Exclude = Qt::Unchecked,
//        Request = Qt::PartiallyChecked,
//        Require = Qt::Checked
//    };

    explicit Charset(bool useExtendedAscii, bool excludeChars, const QString& excludedChars,
                     int usePunctuation, int useDigits, int useUpperAlpha,
                     int useLowerAlpha);

    Charset(const Charset&) = delete;
    Charset(const Charset&&) = delete;

    Charset& operator=(const Charset&) = delete;
    Charset& operator=(const Charset&&) = delete;

    QString allChars() const;

    QString punctChars() const;

    QString digitChars() const;

    QString upperAlphaChars() const;

    QString lowerAlphaChars() const;

private:
    void makeCharString();

    bool mUseExtendedAscii;
    bool mExcludeChars;
    QString mExcludedChars;
    int mUsePunctuation;
    int mUseDigits;
    int mUseUpperAlpha;
    int mUseLowerAlpha;

    QString mAllChars;
    QString mPunctChars;
    QString mDigitChars;
    QString mUpperAlphaChars;
    QString mLowerAlphaChars;
};

#endif // CHARSET_H
