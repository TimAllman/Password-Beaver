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

#include "PasswordGenerator.h"
#include "CharacterPool.h"
#include "Global.h"
#include "Exceptions.h"
#include "OptionsManager.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>

#include <chrono>
#include <vector>
#include <algorithm>

PasswordGenerator::PasswordGenerator()
{
    // We use the std::random_device to seed our local PRNG.
    // Each time this code is ported the quality of the supplied
    // std::random_device should be checked as some may not have
    // access to hardware or may just be poorly implemented.
    // Note that std::random_device throws on construction or on calling
    // ::operator() if the system has no means to access a stochastic
    // process to use to provide a good random number.
#if defined(__linux__)
    std::random_device rd;
    mGenerator.seed(rd());
#elif defined(__FreeBSD__)
    std::random_device rd;
    mGenerator.seed(rd());
#elif defined(_WIN32)
    std::random_device rd;
    mGenerator.seed(rd());
#elif defined(__APPLE__)
    std::random_device rd;
    mGenerator.seed(rd());
#else
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
        if (string.contains(*iter))
            ++count;

    return count;
}

QString PasswordGenerator::password()
{
    OptionsManager& optsMan = OptionsManager::instance();
    CharacterPool charSet(optsMan.useExtendedAscii(), optsMan.charsToExclude(),
                          optsMan.usePunctuation(), optsMan.useDigits(),
                          optsMan.useUpperAlpha(), optsMan.useLowerAlpha(),
                          optsMan.useSymbols());

    // Because character classes, especially the small ones (symbols, punctuation & digits),
    // can be underrepresented we start by adding one member of each required class to the password
    // to begin with but do nothing if they are merely allowed or excluded.
    QString password;
    int numExtraChars = 0;

    if (optsMan.useUpperAlpha() == CharacterPool::REQUIRE)
    {
        unsigned idx = randomIndex(charSet.upperAlphaChars().length() - 1);
        password += charSet.upperAlphaChars().at(idx);
        ++numExtraChars;
    }

    if (optsMan.useLowerAlpha() == CharacterPool::REQUIRE)
    {
        unsigned idx = randomIndex(charSet.lowerAlphaChars().length() - 1);
        password += charSet.lowerAlphaChars().at(idx);
        ++numExtraChars;
    }

    if (optsMan.useSymbols() == CharacterPool::REQUIRE)
    {
        unsigned idx = randomIndex(charSet.symbolChars().length() - 1);
        password += charSet.symbolChars().at(idx);
        ++numExtraChars;
    }

    if (optsMan.usePunctuation() == CharacterPool::REQUIRE)
    {
        unsigned idx = randomIndex(charSet.punctChars().length() - 1);
        password += charSet.punctChars().at(idx);
        ++numExtraChars;
    }

    if (optsMan.useDigits() == CharacterPool::REQUIRE)
    {
        unsigned idx = randomIndex(charSet.digitChars().length() - 1);
        password += charSet.digitChars().at(idx);
        ++numExtraChars;
    }

    QString allChars = charSet.allChars();
    int charSetLength = allChars.size();

    if (charSetLength < Global::MIN_POOL_LENGTH)
    {
        QString len;
        len.setNum(charSetLength);
        QString str = QString(QObject::tr("The character pool has too few (%1) characters.")).arg(len);
        SmallCharacterPoolException ex(QObject::tr(str.toUtf8()));
        ex.raise();
    }

    // This is not elegant but is sufficient.
    int pwLength = optsMan.passwordLength() - numExtraChars;
    int nLoops = 0;   // for debugging only
    do
    {
        ++nLoops;
        for (int i = 0; i < pwLength; ++i)
        {
            unsigned idx = randomIndex(charSetLength - 1);
            password += allChars[idx];
        }
    }
    while (!isValidPassword(password, charSet));

    // shuffle the password so that the extra characters are not all at the front.
    password = shufflePassword(password);

    if (nLoops > 1)
        qInfo() << "Valid password found after " << nLoops << " tries.";

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

QString PasswordGenerator::shufflePassword(const QString& password)
{
    std::vector<QChar> vec;
    for (auto iter = password.cbegin(); iter != password.cend(); ++iter)
        vec.push_back(*iter);

    std::shuffle(vec.begin(), vec.end(), mGenerator);

    QString newPw;
    for (auto iter = vec.cbegin(); iter != vec.cend(); ++iter)
        newPw.append(*iter);

    return newPw;
}

bool PasswordGenerator::isValidPassword(const QString& password, const CharacterPool& charSet) const
{
    OptionsManager& optsMan = OptionsManager::instance();
    QRegularExpression re;
    QRegularExpressionMatch match;

    if (optsMan.useSymbols() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.symbolChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (optsMan.usePunctuation() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.punctChars());
        if (count == 0)
            return false;
    }

    if (optsMan.useDigits() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.digitChars());
        if (count == 0)
            return false;
    }

    if (optsMan.useLowerAlpha() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.lowerAlphaChars());
        if (count == 0)
            return false;
    }

    if (optsMan.useUpperAlpha() == CharacterPool::REQUIRE)
    {
        unsigned count = contains(password, charSet.upperAlphaChars());
        if (count == 0)
            return false;
    }

    return true;
}

