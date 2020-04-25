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

#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <random>

#include <QString>

#include "CharacterPool.h"

class PasswordGenerator
{
public:
    /**
     * This class creates a random password string. Use it as a function object by
     * calling the function call method.
     */
    explicit PasswordGenerator();

    /**
     * Create a password string based upon parameters in the @c Settings instance.
     * @return The generated password string.
     * @exception std::runtime-error Will throw if the user's request cannot be
     * satisfied. This can happen if a class of characters is REQUIREd but has been
     * manually excluded.
     */
    QString password();

    /**
     * Get the entropy of the current password.
     * @return The calculated entropy.
     */
    double entropy() const;

private:
    /**
     * Generates and returns a random index in the range [0..maxIdx].
     * All indices in the range are equally likely.
     * @param maxIdx The highest index to return.
     * @return Random index in the range [0..maxIdx].
     */
    unsigned randomIndex(unsigned maxIdx) const;

    unsigned contains(const QString& string, const QString& chars) const;

    /**
     * Checks to see if @c password meets the criteria for a
     * valid password. It returns @c true if @c password contains at least one
     * character from each of the character types REQUIREd.
     * @param charSet The set of characters to choose from.
     * @param password The candidate password to check.
     * @return @c true on success, false otherwise.
     */
    bool validPassword(const CharacterPool& charSet, const QString& password) const;

    /**
     * Calculate the entropy of the password.
     * @param passwordLength The length of the password.
     * @param charSetLength The length of the source character set.
     * @return The entropy.
     */
    double calcEntropy(int passwordLength, int charSetLength);

    /**
     * An implementation of the Mersenne Twister random number generator.
     * https://en.wikipedia.org/wiki/Mersenne_Twister
     * It is seeded in a way suitable for the system. Under Linux
     * we use `std::random_device`.
     * This object changes state when seeded and when used but this is of no
     * interest here so it is marked `mutable`.
     */
    mutable std::mt19937 mGenerator;

    double mEntropy; ///< Stores the last calculated entropy.
};

#endif // STRINGGENERATOR_H

