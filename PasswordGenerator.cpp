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
#include <QtDebug>

#include <string>
#include <random>
#include <chrono>
#include <cstddef>

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

QString PasswordGenerator::password()
{
    OptionsManager& optsMan = OptionsManager::instance();

    CharacterPool charSet(optsMan.useExtendedAscii(), optsMan.excludeCharacters(),
                    optsMan.charsToExclude(), optsMan.usePunctuation(),
                    optsMan.useDigits(), optsMan.useUpperAlpha(),
                    optsMan.useLowerAlpha(), optsMan.useSymbols());

    int pwLength = optsMan.passwordLength();
    QString password;

    QString allChars = charSet.allChars();
    int charSetLength = allChars.size();

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
        re.setPattern("[" + charSet.punctChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (optsMan.useDigits() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.digitChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (optsMan.useLowerAlpha() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.lowerAlphaChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (optsMan.useUpperAlpha() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.upperAlphaChars() + "]+");
        QRegularExpressionMatch match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    return true;
}
