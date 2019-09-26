//
//  PasswordGenerator.cpp
//  Password Maker
//

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

#include "PasswordGenerator.h"
#include "CharacterPool.h"
#include "Settings.h"

PasswordGenerator::PasswordGenerator()
{
    // We use the std::random_device to seed our local PRNG.
    // Each time this code is ported the quality of the supplied
    // std::random_device should be checked as some may not have
    // access to hardware or may just be poorly implemented.
    std::random_device rd;
    mGenerator.seed(rd());
}

unsigned PasswordGenerator::randomIndex(unsigned maxIdx) const
{
    std::uniform_int_distribution<unsigned> distribution(0, maxIdx);
    unsigned retval = distribution(mGenerator);
    return retval;
}

QString PasswordGenerator::password()
{
    Settings& settings = Settings::instance();

    CharacterPool charSet(settings.useExtendedAscii(), settings.excludeCharacters(),
                    settings.charactersToExclude(), settings.usePunctuation(),
                    settings.useDigits(), settings.useUpperAlpha(),
                    settings.useLowerAlpha());

    QString allChars = charSet.allChars();

    int pwLength = settings.passwordLength();
    QString password;

    auto charSetLength = static_cast<unsigned>(allChars.size());
    //TODO fix this
    if (charSetLength == 0)
        return password;

    do
    {
        password = "";
        for (int i = 0; i < pwLength; ++i)
        {
            unsigned idx = randomIndex(charSetLength - 1);
            password += allChars[idx];
        }
    }
    while (!validPassword(charSet, password));

    mEntropy = calcEntropy(password.length(), allChars.length());

    return password;
}

double PasswordGenerator::entropy() const
{
    return mEntropy;
}

double PasswordGenerator::calcEntropy(int passwordLength, int charSetLength)
{
    // We are using a 64 bit random number generator so the entropy is
    // limited to 64 bits so we return at most 64.0.
    double entr = passwordLength * log2(charSetLength);
    return entr;// > 64.0 ? 64.0 : entr;
}

bool PasswordGenerator::validPassword(const CharacterPool& charSet, const QString& password) const
{
    Settings& settings = Settings::instance();
    QRegularExpression re;
    QRegularExpressionMatch match;

    if (settings.usePunctuation() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.punctChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useDigits() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.digitChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useLowerAlpha() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.lowerAlphaChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useUpperAlpha() == CharacterPool::REQUIRE)
    {
        re.setPattern("[" + charSet.upperAlphaChars() + "]+");
        QRegularExpressionMatch match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    return true;
}






