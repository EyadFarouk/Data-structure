#include "Trie.h"
using namespace std;
bool Trie::deleteHelper(TrieNode *node, const string &word, int index) {
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

void Trie::collectWords(TrieNode* node, string currentWord, vector<string>& result) {
    if (node->isEndOfWord) {
        result.push_back(currentWord);
    }
    for (auto& pair : node->children) {
        collectWords(pair.second, currentWord + pair.first, result);
    }
}


Trie::Trie() {
    root = new TrieNode();
    root->isEndOfWord = false;
}

Trie::~Trie() {
    clear();
}
void Trie::deleteWord(const string &word) {
    deleteHelper(root, word, 0);
}

void Trie::clear() {
    clearHelper(root);
}
/**
 * function to add word to trie
 * @param word 
 * * */
void Trie::insert(string word)
{
    TrieNode* node = root;
    for (char ch : word){
        if (!node->children.count(ch))
            node->children[ch]=new TrieNode();
        node=node->children[ch];
    }
    node->isEndOfWord = true;

}

bool Trie::search(string word) {
    TrieNode* node = root;
    for (char c : word) {
        if (!node->children.count(c))
            return false;
        node = node->children[c];
    }
    return node->isEndOfWord;
}

bool Trie::startsWith(string prefix) {
    TrieNode* node = root;
    for (char c : prefix) {
        if (!node->children.count(c))
            return false;
        node = node->children[c];
    }
    return true;
}

vector<string> Trie::getWords(string prefix) {
    vector<string> result;
    TrieNode* node = root;

    for (char c : prefix) {
        if (!node->children.count(c)) {
            return result; 
        }
        node = node->children[c];
    }

    collectWords(node, prefix, result);

    return result;
}

vector<string> Trie::bfsWords(string prefix) {
    vector<string> result;
    TrieNode* node = root;

    for (char c : prefix) {
        if (!node->children.count(c)) {
            return result;
        }
        node = node->children[c];
    }

    queue<pair<TrieNode*, string>> q;
    q.push(make_pair(node, prefix));

    while (!q.empty()) {
        pair<TrieNode*, string> current = q.front();
        q.pop();

        TrieNode* currentNode = current.first;
        string currentWord = current.second;

        if (currentNode->isEndOfWord) {
            result.push_back(currentWord);
        }

        for (map<char, TrieNode*>::iterator it = currentNode->children.begin(); it != currentNode->children.end(); ++it) {
            char childChar = it->first;
            TrieNode* childNode = it->second;
            q.push(make_pair(childNode, currentWord + childChar));
        }
    }

    return result;
}
