#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <random>

#include <QString>

#include "Charset.h"

class PasswordGenerator
{
public:
    /**
     * @brief StringGenerator
     *
     * This class creates a random password string. Use it as a function object by
     * calling the function call method.
     */
    explicit PasswordGenerator();

    /**
     * @brief randomIndex
     *
     * Generates and returns a random index in the range [0..maxIdx]. All indices
     * in the range are equally likely.
     * @param maxIdx The highest index to return.
     * @return Random index in the range [0..maxIdx].
     */
    unsigned randomIndex(unsigned maxIdx) const;

    /**
     * @brief password()
     *
     * Create a password string of length @c length based upon the
     * characters passed in @c charSet. Use it as a function object by calling the
     * function call method.
     * @return The generated password string.
     */
    QString password();

    double entropy() const;


private:
    /**
     * Checks to see if @c password meets the criteria for a
     * valid password. It returns @c true if @c password contains at least one
     * character from each of the character types requested.
     * @param password The candidate password to check.
     * @return @c true on success, false otherwise.
     */
    bool validPassword(const Charset& charSet, const QString& password) const;

    /**
     * @brief mGenerator
     * An implementation of the Mersenne Twister random number generator.
     * https://en.wikipedia.org/wiki/Mersenne_Twister
     * It is seeded from the system clock.
     * This object changes state when seeded and when used but this is of no
     * interest here so it is marked @c mutable.
     */
    mutable std::mt19937_64 mGenerator;

    double calcEntropy(int passwordLength, int charSetLength);

    double mEntropy;
};

#endif // STRINGGENERATOR_H

