/*
 * Copyright © 2020 Tim Allman
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

#ifndef OPTIONS_H
#define OPTIONS_H


#include <QString>
#include <QMap>
#include <QJsonObject>

class OptionsManager
{
public:
    // These are used when a JSON object is created to save the options.
    static const QString STR_OPTIONS;
    static const QString STR_DEFAULT;
    static const QString STR_NAME;
    static const QString STR_ACTIVE_KEY;
    static const QString STR_CHARS_TO_EXCLUDE;
    static const QString STR_USE_EXTENDED_ASCII;
    static const QString STR_USE_PUNCTUATION;
    static const QString STR_USE_SYMBOLS;
    static const QString STR_USE_DIGITS;
    static const QString STR_USE_LOWER_ALPHA;
    static const QString STR_USE_UPPER_ALPHA;
    static const QString STR_PASSWORD_LENGTH;
    static const QString STR_COPY_TO_CLIPBOARD;
    static const QString STR_CLIPBOARD_CLEAR_TIME;
private:
    /**
     * @brief The OptionsSet struct.
     * This is the struct used to contain a set of options.
     */
    struct OptionsSet
    {
        QString mName;           ///< The user assigned name of the option set.
        bool mUseExtendedAscii;        ///< Include extended ASCII characters in the password if true.
        int mUsePunctuation;     ///< Include punctuation characters in the password if true.
        int mUseSymbols;         ///< Include symbol characters in the password if true.
        int mUseDigits;          ///< Include digit characters in the password if true.
        int mUseUpperAlpha;      ///< Include upper case alpha characters in the password if true.
        int mUseLowerAlpha;      ///< Include lower case alpha characters in the password if true.
        QString mCharsToExclude; ///< Specific characters to exclude.
        int mPasswordLength;     ///< The requested length of the password.
        bool mCopyToClipboard;   ///< Copy result to clipboard if true.
        int mClipboardClearTime; ///< Delay before clearing clipboard. 0 indicates not to clear.

        /**
         * Constructor.
         */
        OptionsSet();

        /**
         * Initialise the instance.
         */
        void setToDefault();

        /**
         * Fill up the QJsonObject with the contents of this.
         * @param jsonObj The object to fill.
         */
        void writeToJSON(QJsonObject& jsonObj) const;

        /**
         * Load the data in the object.
         * @param jsonObj The object to read from.
         */
        void readFromJSON(const QJsonObject& jsonObj);

        /**
         * Compares two instances for all members that affect password
         * generation. The members that affect other aspects of the program
         * (@c mName, @c mCopyToClipboard & @c mClipboardClearTime are not
         * part of the comparison.
         * @param other The other instance.
         * @return @c true if all compared members are equal.
         * @c false otherwise.
         */
        bool comparePasswordOptions(const OptionsSet& other) const;
    };

    /**
     * Equality operator. This works as expected. @see OptionsSet::compareOptions
     * and the implementation code.
     * @param lhs The instance on the left side.
     * @param rhs The instance on the right side.
     * @return true if both instances are identical.
     */
    friend bool operator==(const OptionsSet& lhs, const OptionsSet& rhs);

    /**
     * Inequality operator. This works as expected. @see OptionsSet::compareOptions
     * and the implementation code.
     * @param lhs The instance on the left side.
     * @param rhs The instance on the right side.
     * @return true if both instances are different.
     */
    friend bool operator!=(const OptionsSet& lhs, const OptionsSet& rhs);

    /**
     * ListType. A convenient container type for storing the option sets.
     */
    typedef QMap<QString, OptionsSet> ListType;

public:
    /**
     * The only way of getting an instance.
     * @return Reference to the instance.
     */
    static OptionsManager& instance();

///@{
/// Getters and setters.
    void setActiveKey(const QString& key);
    QString activeKey();

    void setUseExtendedAscii(bool useExtendedAscii);
    bool useExtendedAscii() const;

    void setUsePunctuation(int usePunct);
    int usePunctuation() const;

    void setUseSymbols(int useSymbols);
    int useSymbols() const;

    void setUseDigits(int useDigits);
    int useDigits() const;

    void setUseUpperAlpha(int useUpper);
    int useUpperAlpha() const;

    void setUseLowerAlpha(int useLower);
    int useLowerAlpha() const;

    void setCharsToExclude(const QString& chars);
    QString charsToExclude() const;

    void setPasswordLength(int length);
    int passwordLength() const;

    void setCopyToClipboard(bool copy);
    bool copyToClipboard() const;

    void setClipboardClearTime(int time);
    int clipboardClearTime() const;

    void writeToJSON(QJsonObject& jsonObject) const;
    void readFromJSON(const QJsonObject& jsonObject);

    void setName(const QString& name);
    QString name() const;
///@}

    /**
     * Fills up an instance with a standard set of options.
     */
    void addDefault();

    /**
     * See if this instance contains an OptionSet keyed to @c name in @c mOptionsMap.
     * @param name The name of the option set. Used as a key on the map.
     * @return @c true if this record exists, @c false otherwise.
     */
    bool contains(const QString& name);

    /**
     * Check to see if mActiveOptions contains a record with the same name/key and if
     * that record is identical with the active one.
     * @param name The name of the record. Also used as a key in mOptionsMap.
     * @return @c true if the record exists and is identical with mActiveOptions.
     */
    bool isModified(const QString&name);

    /**
     * Get the list of names/keys in the map.
     * @return The list of keys.
     */
    QStringList names();

    /**
     * Make the record keyed to @c name the active one.
     * @param name The record we want to be the active one.
     */
    void setActive(const QString& name);

    /**
     * Store the current active options in the map. If @c name does not exist a new record
     * will be created. Otherwise the existing one will be overwritten.
     * @param newName
     */
    void saveOptions(const QString& newName);

    /**
     * Delete a record from the map.
     * @param name The name/key of the record to delete.
     */
    void deleteOptions(const QString& name);

private:
    /**
     * Constructor. Private because we are using this class as a singleton.
     */
    OptionsManager();

    /**
     * Default destructor.
     */
    ~OptionsManager() = default;

    /**
     * Deleted copy constructor.
     */
    OptionsManager(const OptionsManager&) = delete;

    QString mActiveOptionsKey;  ///< The key of the currently active option set.
    ListType mOptionsMap;       ///< Map to contain the data.
    OptionsSet mActiveOptions;  ///< The buffer of current options.
};

/**
 * Comparison operator.
 * @param lhs The instance on the left hand side of the operator.
 * @param rhs The instance on the right hand side of the operator.
 * @return @code true if they are the same, @c false otherwise.
 * Calls OptionsSet::compareOptions().
 */
bool operator==(const OptionsManager::OptionsSet& lhs, const OptionsManager::OptionsSet& rhs);

/**
 * Comparison operator.
 * @param lhs The instance on the left hand side of the operator.
 * @param rhs The instance on the right hand side of the operator.
 * @return @code false if they are the same, @c true otherwise.
 * Calls OptionsSet::compareOptions().
 */
bool operator!=(const OptionsManager::OptionsSet& lhs, const OptionsManager::OptionsSet& rhs);

#endif // OPTIONS_H
