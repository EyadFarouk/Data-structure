#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
    root->isEndOfWord = false;
    for (int i = 0; i < 26; i++) {
        root->children[i] = nullptr;
    }
}

Trie::~Trie() {
    // TODO: Implement proper cleanup of all nodes
    delete root;
} 