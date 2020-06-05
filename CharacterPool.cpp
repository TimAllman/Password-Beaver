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

#include <string>
#include <random>

#include "CharacterPool.h"
#include "Exceptions.h"

CharacterPool::CharacterPool(bool useUnicode, const QString& excludedChars,
                             int usePunctuation, int useDigits, int useUpperAlpha,
                             int useLowerAlpha, int useSymbols)
    : mUseUnicode(useUnicode), mExcludedChars(excludedChars),
    mUsePunctuation(usePunctuation), mUseDigits(useDigits), mUseUpperAlpha(useUpperAlpha),
    mUseLowerAlpha(useLowerAlpha), mUseSymbols(useSymbols)
{
    makeCharacterSet();
}

void CharacterPool::makeCharacterSet()
{
    // Select the ASCII characters only.
    int lastChar = 0x7F;

    // Use the ISO/IEC 8859-1 (Latin-1 Extension) character set if wanted.
    if (mUseUnicode)
        lastChar = 0xFF;

    for(int c = 0; c <= lastChar; ++c)
    {
        QChar ch(c);
        if ((ch.isPrint() && !ch.isSpace()) &&
            (!mExcludedChars.contains(ch, Qt::CaseSensitive)))
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

    if ((mUseSymbols == REQUIRE) && (mSymbolChars.length() == 0))
        throw std::runtime_error("Symbol characters are required but have been excluded.");

    if ((mUsePunctuation == REQUIRE) && (mPunctChars.length() == 0))
        throw std::runtime_error("Punctuation characters are required but have been excluded.");

    if ((mUseDigits == REQUIRE) && (mDigitChars.length() == 0))
        throw std::runtime_error("Digit characters are required but have been excluded.");

    if ((mUseLowerAlpha == REQUIRE) && (mLowerAlphaChars.length() == 0))
        throw std::runtime_error("Lower case characters are required but have been excluded.");

    if ((mUseUpperAlpha == REQUIRE) && (mUpperAlphaChars.length() == 0))
        throw std::runtime_error("Upper case characters are required but have been excluded.");

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

void CharacterPool::redistribute()
{
    float allSize = mAllChars.length();
    float digitSize = mDigitChars.length();
    float puncSize = mPunctChars.length();
    float symbolSize = mSymbolChars.length();

    int charsAdded = 0;

    if (digitSize / allSize < 0.1)
    {
        charsAdded += mDigitChars.length();
        mDigitChars += mDigitChars;
    }

    if (puncSize / allSize < 0.1)
    {
        charsAdded += mPunctChars.length();
        mPunctChars += mPunctChars;
    }

    if (symbolSize / allSize < 0.1)
    {
        charsAdded += mSymbolChars.length();
        mSymbolChars += mSymbolChars;
    }
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

