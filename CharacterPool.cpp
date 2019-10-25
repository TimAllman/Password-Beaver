#include <string>
#include <random>

#include "CharacterPool.h"
#include "Exceptions.h"

CharacterPool::CharacterPool(bool useExtendedAscii, bool excludeChars, const QString& excludedChars,
                 int usePunctuation, int useDigits, int useUpperAlpha,
                 int useLowerAlpha, int useSymbols)
    : mUseExtendedAscii(useExtendedAscii), mExcludeChars(excludeChars), mExcludedChars(excludedChars),
      mUsePunctuation(usePunctuation), mUseDigits(useDigits), mUseUpperAlpha(useUpperAlpha),
    mUseLowerAlpha(useLowerAlpha), mUseSymbols(useSymbols)
{
    makeCharacterSet();
}

//bool CharacterPool::isSymbol(const QChar& c)
//{
//    QChar::Category cat = c.category();

//    if (cat == QChar::Symbol_Math ||
//        cat == QChar::Symbol_Currency ||
//        cat == QChar::Symbol_Modifier ||
//        cat == QChar::Symbol_Other)
//        return true;
//    else
//        return false;
//}

//bool CharacterPool::isPunctuation(const QChar& c)
//{
//    QChar::Category cat = c.category();

//    if (cat == QChar::Punctuation_Connector
//        || cat == QChar::Punctuation_Dash
//        || cat == QChar::Punctuation_Open
//        || cat == QChar::Punctuation_Close
//        || cat == QChar::Punctuation_InitialQuote
//        || cat == QChar::Punctuation_FinalQuote
//        || cat == QChar::Punctuation_Other)
//        return true;
//    else
//        return false;
//}

void CharacterPool::makeCharacterSet()
{
    //
    int lastChar = 127;
    if (mUseExtendedAscii)
        lastChar = 255;

    for(int c = 0; c <= lastChar; ++c)
    {
        QChar ch(c);
        if ((ch.isPrint() && !ch.isSpace()) &&
            (!(mExcludeChars && mExcludedChars.contains(ch, Qt::CaseSensitive))))
        {
            //            if (isPunctuation(ch))
            //                mPunctChars += ch;
            //            else if (isSymbol(ch))
            //                mSymbolChars += ch;

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

    if ((mExcludeChars && mUseSymbols == REQUIRE) && (mSymbolChars.length() == 0))
        throw std::runtime_error("Symbol characters are required but have been excluded.");

    if ((mExcludeChars && mUsePunctuation == REQUIRE) && (mPunctChars.length() == 0))
        throw std::runtime_error("Punctuation characters are required but have been excluded.");

    if ((mExcludeChars && mUseDigits == REQUIRE) && (mDigitChars.length() == 0))
        throw std::runtime_error("Digit characters are required but have been excluded.");

    if ((mExcludeChars && mUseLowerAlpha == REQUIRE) && (mLowerAlphaChars.length() == 0))
        throw std::runtime_error("Lower case characters are required but have been excluded.");

    if ((mExcludeChars && mUseUpperAlpha == REQUIRE) && (mUpperAlphaChars.length() == 0))
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
