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
#include <chrono>
#include <cstddef>

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtDebug>

#include "PasswordGenerator.h"
#include "CharacterPool.h"
#include "Settings.h"
#include "Global.h"
#include "Exceptions.h"

PasswordGenerator::PasswordGenerator()
{
    // We use the std::random_device to seed our local PRNG.
    // Each time this code is ported the quality of the supplied
    // std::random_device should be checked as some may not have
    // access to hardware or may just be poorly implemented.
    // Note that std::random_device throws on construction or on calling
    // ::operator() if the system has no means to access a stochastic
    // process to use to provide a good random number.
#if defined(linux)
    std::random_device rd;
    mGenerator.seed(rd());
#elif defined(_WIN32)
    std::random_device rd;
    mGenerator.seed(rd());
#elif defined(__APPLE__)
    std::random_device rd;
    mGenerator.seed(rd());
#endif

}

unsigned PasswordGenerator::randomIndex(unsigned maxIdx) const
{
    std::uniform_int_distribution<unsigned> distribution(0, maxIdx);
    unsigned retval = distribution(mGenerator);
    return retval;
}

unsigned PasswordGenerator::contains(const QString& string, const QString& chars) const
{
    unsigned count = 0;

    // loop through 'chars' and count the number of times a character in 'chars'
    // is found in 'string'. Multiple occurrences of a character are counted as one
    // match.
    for (auto iter = chars.cbegin(); iter != chars.cend(); ++iter)
    {
        if (string.contains(*iter))
        {
            ++count;
        }
    }

    return count;
}

QString PasswordGenerator::password()
{
    Settings& settings = Settings::instance();

    CharacterPool charSet(settings.useExtendedAscii(), settings.excludeCharacters(),
                    settings.charactersToExclude(), settings.usePunctuation(),
                    settings.useDigits(), settings.useUpperAlpha(),
                    settings.useLowerAlpha(), settings.useSymbols());

    int pwLength = settings.passwordLength();
    QString password;

    QString allChars = charSet.allChars();
    auto charSetLength = static_cast<unsigned>(allChars.size());

    if (charSetLength < Global::MIN_POOL_LENGTH)
    {
        QString len;
        len.setNum(charSetLength);
        SmallCharacterPoolException ex("The character pool has too few (" + len + ") characters.");
        ex.raise();
    }

    int nLoops = 0;   // for debugging only
    do
    {
        ++nLoops;
        password = "";
        for (int i = 0; i < pwLength; ++i)
        {
            unsigned idx = randomIndex(charSetLength - 1);
            password += allChars[idx];
        }
    }
    while (!validPassword(charSet, password));

    if (nLoops > 1)
        qDebug() << "Valid password found after " << nLoops << " tries.";

    mEntropy = calcEntropy(password.length(), allChars.length());

    return password;
}

double PasswordGenerator::entropy() const
{
    return mEntropy;
}

double PasswordGenerator::calcEntropy(int passwordLength, int charSetLength)
{
    return passwordLength * log2(charSetLength);
}

bool PasswordGenerator::validPassword(const CharacterPool& charSet, const QString& password) const
{
    Settings& settings = Settings::instance();
    QRegularExpression re;
    QRegularExpressionMatch match;

    if (settings.useSymbols() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.symbolChars());
        if (count == 0)
            return false;
    }

    if (settings.usePunctuation() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.punctChars());
        if (count == 0)
            return false;
    }

    if (settings.useDigits() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.digitChars());
        if (count == 0)
            return false;
    }

    if (settings.useLowerAlpha() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.lowerAlphaChars());
        if (count == 0)
            return false;
    }

    if (settings.useUpperAlpha() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.upperAlphaChars());
        if (count == 0)
            return false;
    }

    return true;
}

//bool PasswordGenerator::validPassword(const CharacterPool& charSet, const QString& password) const
//{
//    Settings& settings = Settings::instance();
//    QRegularExpression re;
//    QRegularExpressionMatch match;

//    if (settings.useSymbols() == CharacterPool::REQUIRE)
//    {
//        QString pattern = "[" + charSet.symbolChars() + "]+";
//        re.setPattern(pattern);
//        pattern = re.pattern();
//        match = re.match(password);
//        if (!match.hasMatch())
//            return false;
//    }

//    if (settings.usePunctuation() == CharacterPool::REQUIRE)
//    {
//        QString pattern = "[" + charSet.punctChars() + "]+";
//        re.setPattern(pattern);
//        pattern = re.pattern();
//        match = re.match(password);
//        if (!match.hasMatch())
//            return false;
//    }

//    if (settings.useDigits() == CharacterPool::REQUIRE)
//    {
//        QString pattern = "[" + charSet.digitChars() + "]+";
//        re.setPattern(pattern);
//        pattern = re.pattern();
//        match = re.match(password);
//        if (!match.hasMatch())
//            return false;
//    }

//    if (settings.useLowerAlpha() == CharacterPool::REQUIRE)
//    {
//        QString pattern = "[" + charSet.lowerAlphaChars() + "]+";
//        re.setPattern(pattern);
//        match = re.match(password);
//        if (!match.hasMatch())
//            return false;
//    }

//    if (settings.useUpperAlpha() == CharacterPool::REQUIRE)
//    {
//        QString pattern = "[" + charSet.upperAlphaChars() + "]+";
//        re.setPattern(pattern);
//        pattern = re.pattern();
//        QRegularExpressionMatch match = re.match(password);
//        if (!match.hasMatch())
//            return false;
//    }

//    return true;
//}






