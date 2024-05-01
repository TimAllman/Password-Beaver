/*
 * Copyright © 2023 Tim Allman
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

#include "CharacterPool.h"

CharacterPool::CharacterPool(bool useLatin1, const QString& excludedChars, int usePunctuation,
                             int useDigits, int useUpperAlpha, int useLowerAlpha, int useSymbols)
    : mUseLatin1(useLatin1), mUsePunctuation(usePunctuation), mUseDigits(useDigits),
    mUseUpperAlpha(useUpperAlpha), mUseLowerAlpha(useLowerAlpha), mUseSymbols(useSymbols),
    mExcludedChars(excludedChars)
{
    makeCharacterSet();
}

void CharacterPool::makeCharacterSet()
{
    const int LAST_ASCII_CHAR = 0x7F;
    const int LAST_LATIN1_CHAR = 0xFF;

    int lastChar;
    if (mUseLatin1)
        lastChar = LAST_LATIN1_CHAR;
    else
        lastChar = LAST_ASCII_CHAR;

    // Here we assemble the character pool from which all passwords are created.
    // The characters must be ordinary printable ones (not white space etc.).
    for(int i = 0; i <= lastChar; ++i)
    {
        QChar ch(i);
        if (ch.isPrint() && !ch.isSpace() && !mExcludedChars.contains(ch, Qt::CaseSensitive))
        {
            if (ch.isPunct())
                mPunctChars += ch;
            else if (ch.isSymbol())
                mSymbolChars += ch;
            else if (ch.isDigit())
                mDigitChars += ch;
            else if (ch.isLower())
                mLowerAlphaChars += ch;
            else if (ch.isUpper())
                mUpperAlphaChars += ch;
        }
    }

    if (mUseDigits)
        mAllChars += mDigitChars;
    if (mUseUpperAlpha)
        mAllChars += mUpperAlphaChars;
    if (mUseLowerAlpha)
        mAllChars += mLowerAlphaChars;
    if (mUsePunctuation)
        mAllChars += mPunctChars;
    if (mUseSymbols)
        mAllChars += mSymbolChars;
}

QString CharacterPool::allChars() const
{
    return mAllChars;
}

int CharacterPool::poolSize() const
{
    return mAllChars.size();
}

QString CharacterPool::symbolChars() const
{
    return mSymbolChars;
}

QString CharacterPool::punctChars() const
{
    return mPunctChars;
}

QString CharacterPool::digitChars() const
{
    return mDigitChars;
}

QString CharacterPool::upperAlphaChars() const
{
    return mUpperAlphaChars;
}

QString CharacterPool::lowerAlphaChars() const
{
    return mLowerAlphaChars;
}

