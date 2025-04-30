#pragma once
#include <unordered_map>
#include <string>
using namespace std;
class Trie {
private:
    struct TrieNode {
        bool isEndOfWord;
        unordered_map<char, TrieNode*> children;
    };
    
    TrieNode* root;
    bool deleteHelper(TrieNode* node, const string& word, int index);
    void clearHelper(TrieNode* node);    
    void collectWords(TrieNode* node, string currentWord, vector<string>& result); 

public:
    Trie();
    ~Trie();
    void insert(string word);
    void deleteWord(const string& word);
    void clear();
    bool search(string word);
    bool startsWith(string prefix);
    vector<string> getWords(string prefix);
    vector<string> bfsWords(string);
};

