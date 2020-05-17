#ifndef OPTIONS_H
#define OPTIONS_H

#include "Settings.h"

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


#include <QString>
#include <QVector>
#include <QJsonObject>

class OptionsManager
{
public:
    OptionsManager();

    OptionsManager(const Settings& settings);

    void setUseExtendedAscii(bool extendedAscii, int index);
    bool useExtendedAscii(int index) const;

    void setUsePunctuation(int usePunct, int index);
    int usePunctuation(int index) const;

    void setUseSymbols(int useSymbols, int index);
    int useSymbols(int index) const;

    void setUseDigits(int useDigits, int index);
    int useDigits(int index) const;

    void setUseUpperAlpha(int useUpper, int index);
    int useUpperAlpha(int index) const;

    void setUseLowerAlpha(int useLower, int index);
    int useLowerAlpha(int index) const;

    void setExcludeCharacters(bool exclude, int index);
    bool excludeCharacters(int index) const;

    void setCharactersToExclude(const QString& chars, int index);
    QString charactersToExclude(int index) const;

    void setPasswordLength(int length, int index);
    int passwordLength(int index) const;

    void setCopyToClipboard(bool copy, int index);
    bool copyToClipboard(int index) const;

    void writeToJSON(QJsonArray& jsonArray);

private:
    struct OptionsRec
    {
        QString mLabel;
        bool mUseExtendedAscii;
        int mUsePunctuation;
        int mUseSymbols;
        int mUseDigits;
        int mUseUpperAlpha;
        int mUseLowerAlpha;
        bool mExcludeCharacters;
        QString mCharactersToExclude;
        int mPasswordLength;
        bool mCopyToClipboard;

        void writeToJSON(QJsonObject& jsonObj);

    };

    int mCurrentOptionsIndex;
    QVector<OptionsRec> mOptionsList;

};

#endif // OPTIONS_H
