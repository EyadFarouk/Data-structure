#include "Trie.h"
bool Trie::deleteHelper(TrieNode *node, const std::string &word, int index) {
    if(index == word.size()) {
        if(!node->isEndOfWord) return false;
        node->isEndOfWord = false;
        return node->children.empty();
    }
    char ch = word[index];
    auto it = node->children.find(ch);
    if(it == node->children.end()) return false;
    bool shouldDelete = deleteHelper(it->second, word, index + 1);
    if(shouldDelete) {
        delete it->second;
        node->children.erase(ch);
    }
    return node->children.empty() && !node->isEndOfWord;
}
void Trie::clearHelper(TrieNode *node) {
    for(auto &child : node->children) {
        clearHelper(child.second);
        delete child.second;
    }
    node->children.clear();
}
Trie::Trie() {
    root = new TrieNode();
    root->isEndOfWord = false;
}

Trie::~Trie() {
    clear();
}
void Trie::deleteWord(const std::string &word) {
    deleteHelper(root, word, 0);
}

void Trie::clear() {
    clearHelper(root);
}
/**
 * function to add word to trie
 * @param word 
 * * */
void Trie::insert(std::string word)
{
    TrieNode*node=root;
    for (char ch : word){
        if (!node->children.count(ch))
            node->children[ch]=new TrieNode();
        node=node->children[ch];
    }
    node->isEndOfWord = true;

}
