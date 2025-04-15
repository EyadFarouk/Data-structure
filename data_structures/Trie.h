#pragma once
#include <unordered_map>
#include <string>
class Trie {
private:
    struct TrieNode {
        bool isEndOfWord;
        std::unordered_map<char, TrieNode*> children;
    };
    
    TrieNode* root;
    bool deleteHelper(TrieNode* node, const std::string& word, int index);
    void clearHelper(TrieNode* node);

public:
    Trie();
    ~Trie();
    void insert(std::string word);
    void deleteWord(const std::string& word);
    void clear();
};

