/*
 * Copyright © 2023 Tim Allman
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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QException>

/**
 * Exception class to be used with character exclusion issues.
 */
class ExclusionException : public QException
{
public:
    /**
     * Public constructor.
     * @param msg Message to describe the problem.
     */
    explicit ExclusionException(const QString& msg)
        : mMessage(msg)
    {}

    /**
     * Copy constructor.
     * @param other The other exception that we wish to copy.
     */
    ExclusionException(const ExclusionException& other)
    {
        mMessage = other.mMessage;
    }

    /**
     * Deleted copy assignment operator.
     * @return nothing
     */
    ExclusionException operator=(const ExclusionException&) = delete;

    /**
     * QException requires this function to be overridden in subclasses.
     * See https://doc.qt.io/qt-5/qexception.html.
     * @return Pointer to a new copy of @c *this.
     */
    ExclusionException* clone() const override
    {
        return new ExclusionException(*this);
    }

    /**
     * QException requires this function to be overridden in subclasses.
     * See https://doc.qt.io/qt-5/qexception.html.
     */
    void raise() const override
    {
        throw *this;
    }

    /**
     * Get the stored message.
     * @return The message.
     */
    QString message() const
    {
        return mMessage;
    }

    /**
     * Destructor.
     */
    ~ExclusionException() override
    {}

private:
    QString mMessage; ///< The message passed to the constructor.
};

/**
 * Exception class to be used with @c CharacterPool exceptions that
 * notify about a character pool which is too small.
 */
class SmallCharacterPoolException : public QException
{
public:
    /**
     * Public constructor.
     * @param msg Message to describe the problem.
     */
    explicit SmallCharacterPoolException(const QString& msg)
        : mMessage(msg)
    {}

    /**
     * Copy constructor.
     * @param other The other exception that we wish to copy.
     */
    SmallCharacterPoolException(const SmallCharacterPoolException& other)
    {
        mMessage = other.mMessage;
    }

    /**
     * Deleted copy assignment operator.
     * @return nothing
     */
    SmallCharacterPoolException operator=(const SmallCharacterPoolException& other) = delete;

    /**
     * QException requires this function to be overridden in subclasses.
     * See https://doc.qt.io/qt-6/qexception.html.
     * @return Pointer to a new copy of @c *this.
     */
    SmallCharacterPoolException* clone() const override
    {
        return new SmallCharacterPoolException(*this);
    }

    /**
     * QException requires this function to be overridden in subclasses.
     * See https://doc.qt.io/qt-5/qexception.html.
     */
    void raise() const override
    {
        throw *this;
    }

    /**
     * Get the stored message.
     * @return The message.
     */
    QString message() const
    {
        return mMessage;
    }

    /**
     * Destructor.
     */
    ~SmallCharacterPoolException() override
    {}

private:
    QString mMessage; ///< The message passed to the constructor.
};

#endif // EXCEPTIONS_H
