#include <stdexcept>
#include <string>
#include <random>
#include <chrono>

#include "Charset.h"

Charset::Charset(bool useExtendedAscii, bool excludeChars, const QString& excludedChars,
                 int usePunctuation, int useDigits, int useUpperAlpha,
                 int useLowerAlpha)
    : mUseExtendedAscii(useExtendedAscii), mExcludeChars(excludeChars), mExcludedChars(excludedChars),
      mUsePunctuation(usePunctuation), mUseDigits(useDigits), mUseUpperAlpha(useUpperAlpha),
      mUseLowerAlpha(useLowerAlpha)
{
    makeCharString();
}

void Charset::makeCharString()
{
    int lastChar = 127;
    if (mUseExtendedAscii)
        lastChar = 255;

    for(int c = 0; c <= lastChar; ++c)
    {
        QChar ch(c);
        if ((ch.isPrint() && !ch.isSpace()) &&
                (!(mExcludeChars && mExcludedChars.contains(ch, Qt::CaseSensitive))))
        {
            if (ch.isPunct())
                mPunctChars += ch;
            else if (ch.isDigit())
                mDigitChars += ch;
            else if (ch.isLower())
                mLowerAlphaChars += ch;
            else if (ch.isUpper())
                mUpperAlphaChars += ch;
        }
    }

    if ((mExcludeChars && mUsePunctuation == REQUIRE) && (mPunctChars.length() == 0))
        throw std::runtime_error("Punctuation characters are required but have been excluded.");

    if ((mExcludeChars && mUseDigits == REQUIRE) && (mDigitChars.length() == 0))
        throw std::runtime_error("Digit characters are required but have been excluded.");

    if ((mExcludeChars && mUseLowerAlpha == REQUIRE) && (mLowerAlphaChars.length() == 0))
        throw std::runtime_error("Lower case characters are required but have been excluded.");

    if ((mExcludeChars && mUseUpperAlpha == REQUIRE) && (mUpperAlphaChars.length() == 0))
        throw std::runtime_error("Upper case characters are required but have been excluded.");

    if (mUsePunctuation)
        mAllChars += mPunctChars;
    if (mUseDigits)
        mAllChars += mDigitChars;
    if (mUseLowerAlpha)
        mAllChars += mLowerAlphaChars;
    if (mUseUpperAlpha)
        mAllChars += mUpperAlphaChars;
}

QString Charset::allChars() const
{
    return mAllChars;
}

QString Charset::punctChars() const
{
    return mPunctChars;
}

QString Charset::digitChars() const
{
    return mDigitChars;
}

QString Charset::upperAlphaChars() const
{
    return mUpperAlphaChars;
}

QString Charset::lowerAlphaChars() const
{
    return mLowerAlphaChars;
}

