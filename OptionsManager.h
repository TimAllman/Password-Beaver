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
    };

    typedef QMap<QString, OptionsSet> ListType;

    /**
     * The only way of getting an instance.
     * @return Reference to the instance.
     */
    static OptionsManager& instance();

    void setDefaults();

    bool contains(const QString& name);

    void setCurrentKey(const QString& key);
    QString currentKey();

    void setUseExtendedAscii(bool extendedAscii, QString key = "");
    bool useExtendedAscii(QString key = "") const;

    void setUsePunctuation(int usePunct, QString key = "");
    int usePunctuation(QString key = "") const;

    void setUseSymbols(int useSymbols, QString key = "");
    int useSymbols(QString key = "") const;

    void setUseDigits(int useDigits, QString key = "");
    int useDigits(QString key = "") const;

    void setUseUpperAlpha(int useUpper, QString key = "");
    int useUpperAlpha(QString key = "") const;

    void setUseLowerAlpha(int useLower, QString key = "");
    int useLowerAlpha(QString key = "") const;

    void setCharsToExclude(const QString& chars, QString key = "");
    QString charsToExclude(QString key = "") const;

    void setPasswordLength(int length, QString key = "");
    int passwordLength(QString key = "") const;

    void setCopyToClipboard(bool copy, QString key = "");
    bool copyToClipboard(QString key = "") const;

    void writeToJSON(QJsonObject& jsonObject) const;
    void readFromJSON(const QJsonObject& jsonObject);

    void setName(const QString& name, QString key = "");
    QString name(QString key = "") const;

private:
    OptionsManager();
    ~OptionsManager() = default;
    OptionsManager(const OptionsManager&) = delete;

    QString mCurrentOptionsKey;
    ListType mOptionsMap;
};

#endif // OPTIONS_H
