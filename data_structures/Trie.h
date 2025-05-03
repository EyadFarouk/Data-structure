#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
using namespace std;
class Trie {
private:
    struct TrieNode {
        bool isEndOfWord;
        unordered_map<char, TrieNode*> children;
    };
    
    TrieNode* root;
    bool deleteHelper(TrieNode* node, const std::string& word, int index);
    void clearHelper(TrieNode* node);
    TrieNode* traverse(const std::string& prefix) const;
    void dfs(TrieNode* node, const std::string& path, std::vector<std::string>& out) const;
public:
    Trie();
    ~Trie();
    void insert(std::string& word);
    void deleteWord(const std::string& word);
    void clear();
    std::vector<std::string> autocomplete(const std::string& prefix) const;
    std::vector<std::string> autocomplete(const std::string& prefix, const bool& bfs) const;
    void collectWords(TrieNode* node, string currentWord, vector<string>& result); 
    bool search(string word);
    bool startsWith(string prefix);
    vector<string> getWords(string prefix);
};

