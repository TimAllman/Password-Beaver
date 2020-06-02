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
    static const QString STR_OPTIONS;
    static const QString STR_DEFAULT;
    static const QString STR_NAME;
    static const QString STR_CURRENT_KEY;
    static const QString STR_CHARS_TO_EXCLUDE;
    static const QString STR_USE_EXTENDED_ASCII;
    static const QString STR_USE_PUNCTUATION;
    static const QString STR_USE_SYMBOLS;
    static const QString STR_USE_DIGITS;
    static const QString STR_USE_LOWER_ALPHA;
    static const QString STR_USE_UPPER_ALPHA;
    static const QString STR_PASSWORD_LENGTH;
    static const QString STR_COPY_TO_CLIPBOARD;

private:
    struct OptionsSet
    {
        QString mName;
        bool mUseExtendedAscii;
        int mUsePunctuation;
        int mUseSymbols;
        int mUseDigits;
        int mUseUpperAlpha;
        int mUseLowerAlpha;
        QString mCharsToExclude;
        int mPasswordLength;
        bool mCopyToClipboard;

        OptionsSet();
        void setToDefault();
        void writeToJSON(QJsonObject& jsonObj) const;
        void readFromJSON(const QJsonObject& jsonObj);

        /**
         * Compares two instances for all members except mName.
         * The @c mName is not an option in the same way that the others
         * that affect the production pd passwords are and often we
         * want to compare everything but the name. Inclusion of @c mName
         * in the struct is a convenience that reflects this field's
         * inclusion in the JSON representation that probably avoids more
         * problems than it creates.
         *
         * @param other The other instance.
         * @return true if all members (other than @c mName are equal.
         * @c false otherwise.
         */
        bool compareOptions(const OptionsSet& other) const;
    };

    friend bool operator==(const OptionsSet& lhs, const OptionsSet& rhs);
    friend bool operator!=(const OptionsSet& lhs, const OptionsSet& rhs);

    typedef QMap<QString, OptionsSet> ListType;

public:
    /**
     * The only way of getting an instance.
     * @return Reference to the instance.
     */
    static OptionsManager& instance();

    void setDefaults();

    bool contains(const QString& name);

    void setCurrentKey(const QString& key);
    QString currentKey();

    void setUseExtendedAscii(bool extendedAscii);
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

    void writeToJSON(QJsonObject& jsonObject) const;
    void readFromJSON(const QJsonObject& jsonObject);

    void setName(const QString& name);
    QString name() const;

    bool isModified(const QString&name);

    QStringList names();

    void setActive(const QString& name);

    void saveOptions(const QString& newName);

    void deleteOptions(const QString& name);

private:
    OptionsManager();
    ~OptionsManager() = default;
    OptionsManager(const OptionsManager&) = delete;

    QString mActiveOptionsKey;
    ListType mOptionsMap;
    OptionsSet mActiveOptions;
};

bool operator==(const OptionsManager::OptionsSet& lhs, const OptionsManager::OptionsSet& rhs);

bool operator!=(const OptionsManager::OptionsSet& lhs, const OptionsManager::OptionsSet& rhs);

#endif // OPTIONS_H
