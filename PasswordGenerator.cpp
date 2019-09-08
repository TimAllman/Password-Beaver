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

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "PasswordGenerator.h"
#include "Charset.h"
#include "Settings.h"

PasswordGenerator::PasswordGenerator()
{
    // construct the default random generator engine from a time-based seed:
    size_t seed = static_cast<size_t>(std::chrono::system_clock::now().time_since_epoch().count());
    mGenerator.seed(seed);
}

unsigned PasswordGenerator::randomIndex(unsigned maxIdx) const
{
    std::uniform_int_distribution<unsigned> distribution(0, maxIdx);
    unsigned retval = distribution(mGenerator);
    return retval;
}

QString PasswordGenerator::password() const
{
    Settings& settings = Settings::instance();

    Charset charSet(settings.useExtendedAscii(), settings.charactersToExclude(),
                    settings.usePunctuation(), settings.useDigits(),
                    settings.useUpperAlpha(), settings.useLowerAlpha());

    QString allChars = charSet.allChars();

    int pwLength = settings.passwordLength();
    QString password;

    auto charSetLength = static_cast<unsigned>(allChars.size());
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

    return password;
}

bool PasswordGenerator::validPassword(const Charset& charSet, const QString& password) const
{
    Settings& settings = Settings::instance();
    QRegularExpression re;
    QRegularExpressionMatch match;

    if (settings.usePunctuation() == Charset::REQUIRE)
    {
        re.setPattern("[" + charSet.punctChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useDigits() == Charset::REQUIRE)
    {
        re.setPattern("[" + charSet.digitChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useLowerAlpha() == Charset::REQUIRE)
    {
        re.setPattern("[" + charSet.lowerAlphaChars() + "]+");
        match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    if (settings.useUpperAlpha() == Charset::REQUIRE)
    {
        re.setPattern("[" + charSet.upperAlphaChars() + "]+");
        QRegularExpressionMatch match = re.match(password);
        if (!match.hasMatch())
            return false;
    }

    return true;
}






