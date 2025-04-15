#pragma once
#include <unordered_map>
class Trie {
private:
    struct TrieNode {
        bool isEndOfWord;
        std::unordered_map<char, TrieNode*> children;
    };
    
    TrieNode* root;

public:
    Trie();
    ~Trie();
};

