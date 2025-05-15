#ifndef REGEXMANAGER_H
#define REGEXMANAGER_H

#include "trie.h"
#include <QRegularExpression>


class RegexManager
{
public:
    RegexManager();
    std::vector<std::string> getWords(const QString &prefix, int mode);

private:
    Trie& trie = Trie::getInstance();
    std::vector<std::string> orderSuggestions(int mode);
    QRegularExpression prefixToRegex(const QString &prefix);


};

#endif // REGEXMANAGER_H
