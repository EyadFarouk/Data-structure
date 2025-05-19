#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "wordscounter.h"

class Trie {
public:
    ~Trie();
    Trie(const Trie&) = delete;
    static Trie& getInstance() {
        static Trie instance;
        return instance;
    }
    void insert(const std::string &word);
    bool remove(const std::string &word);
    bool contains(const std::string &word) const;
    std::vector<std::string> suggestionsDFS(const std::string &prefix, int maxResults = 100) const;
    std::vector<std::string> suggestionsBFS(const std::string &prefix, int maxResults = 100) const;
    std::vector<std::string> suggestionsFreq(const std::string &prefix, int maxResults = 100) const;

private:
    Trie();

    struct Node { Node* children [26]; bool isEnd; Node() : isEnd(false) { for(int i = 0; i < 26; ++i) children[i] = nullptr;} } ;
    Node *root;
    void clear(Node *node);
    bool remove(Node *&node, const std::string &word, int depth);
    void dfs(Node *node, const std::string &prefix, std::vector<std::string> &out, int maxResults) const;
    WordsCounter& wordsCounter = WordsCounter::getInstance();
};

#endif // TRIE_H
