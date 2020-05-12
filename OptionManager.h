#ifndef OPTIONS_H
#define OPTIONS_H

#include "Settings.h"

#include <QString>

struct OptionsRec
{

};

class OptionsManager
{
public:
    OptionsManager();

    OptionsManager(const Settings& settings);

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

    void setExcludeCharacters(bool exclude);
    bool excludeCharacters() const;

    void setCharactersToExclude(const QString& chars);
    QString charactersToExclude() const;

    void setPasswordLength(int length);
    int passwordLength() const;

    void setCopyToClipboard(bool copy);
    bool copyToClipboard() const;

private:
    int mCurrentOptionsIndex;


};

#endif // OPTIONS_H
