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
#include <QVector>
#include <QJsonObject>

class OptionsManager
{
public:
    /**
     * The only way of getting an instance.
     * @return Reference to the instance.
     */
    static OptionsManager& instance();

    void setDefaults();

    void setCurrentIndex(int index);
    int currentIndex();

    void setUseExtendedAscii(bool extendedAscii, int index = -1);
    bool useExtendedAscii(int index = -1) const;

    void setUsePunctuation(int usePunct, int index = -1);
    int usePunctuation(int index = -1) const;

    void setUseSymbols(int useSymbols, int index = -1);
    int useSymbols(int index = -1) const;

    void setUseDigits(int useDigits, int index = -1);
    int useDigits(int index = -1) const;

    void setUseUpperAlpha(int useUpper, int index = -1);
    int useUpperAlpha(int index = -1) const;

    void setUseLowerAlpha(int useLower, int index = -1);
    int useLowerAlpha(int index = -1) const;

    void setCharsToExclude(const QString& chars, int index = -1);
    QString charsToExclude(int index = -1) const;

    void setPasswordLength(int length, int index = -1);
    int passwordLength(int index = -1) const;

    void setCopyToClipboard(bool copy, int index = -1);
    bool copyToClipboard(int index = -1) const;

    void writeToJSON(QJsonObject& jsonObject) const;
    void readFromJSON(const QJsonObject& jsonObject);

private:
    OptionsManager();
    ~OptionsManager() = default;
    OptionsManager(const OptionsManager&) = delete;

    struct OptionsRec
    {
        QString mLabel;
        bool mUseExtendedAscii;
        int mUsePunctuation;
        int mUseSymbols;
        int mUseDigits;
        int mUseUpperAlpha;
        int mUseLowerAlpha;
        QString mCharsToExclude;
        int mPasswordLength;
        bool mCopyToClipboard;

        OptionsRec();
        void setToDefault();
        void writeToJSON(QJsonObject& jsonObj) const;
        void readFromJSON(const QJsonObject& jsonObj);
    };

    typedef QVector<OptionsRec> ListType;

    int mCurrentOptionsIndex;
    ListType mOptionsList;
};

#endif // OPTIONS_H
