#include "regexmanager.h"
#include <QString>

RegexManager::RegexManager() {}

std::vector<std::string> RegexManager::orderSuggestions(int mode) {
    if (mode == 0)
        return trie.suggestionsFreq("", 50);
    else if (mode == 1)
        return trie.suggestionsBFS("", 50);
    return trie.suggestionsDFS("", 50);
}
QRegularExpression RegexManager::prefixToRegex(const QString &prefix) {
    QString pattern = "^";
    for (auto c : prefix) {
        if (c == '_')  pattern += '.';       // SQL “single char” wildcard
        else if (c == '*')  pattern += ".*";      // alternate wildcard if you like
        else                 pattern += QRegularExpression::escape(QString(c));
    }
    return QRegularExpression(pattern,
                              QRegularExpression::CaseInsensitiveOption);
}

std::vector<std::string> RegexManager::getWords(const QString &prefix, int mode) {
    auto allWords = orderSuggestions(mode);
    QRegularExpression regex = prefixToRegex(prefix);
    std::vector<std::string> out;
    for (auto &w : allWords) {
        if (regex.match(QString::fromStdString(w)).hasMatch())
            out.push_back(w);
    }

    return out;
}
