#include "trie.h"
#include <queue>
#include <algorithm>
/*
    Fine, How does it work?
    Great Question!

    Trie has a root -> Represents the empty string ""
    if you add word "ba" the trie will be like this
                        root
                              \
                               b
                                \
                                 a (is end of word)
    then when we add "bab"
                        root
                              \
                               b
                                \
                                 a (is end of word)
                                /
                                b (is end of word)

    and the search takes the bath until getting letter with (is end of word)'s flag
        this is the dfs or (lexografickaly)
    bfs takes the levels keeping in mind that the first word get the (is end of word)'s flag
    is the shortest in length

    * we used const with variables that will not be changed, this make the code more faster

    Section Quote:
            "مسافرٌ أنت والآثار باقية، فاترك وراءك ما تُحيي به أثرك"

*/

Trie::Trie() : root(new Node()) {}
Trie::~Trie() { clear(root); }

// this is a recursive function to delete all sub tries starting from node
void Trie::clear(Node *node) {
    if (!node) return;
    for (int i = 0; i < 26; ++i)
        clear(node->children[i]);
    delete node;
}

// Complixity -> O(n) n is the length of the word
void Trie::insert(const std::string &word) {
    Node *cur = root;
    for (char c : word) {
        if (!cur->children[c - 'a']) cur->children[c - 'a'] = new Node(); // make new trie from this letter
        cur = cur->children[c - 'a'];
    }
    cur->isEnd = true;
}


bool Trie::contains(const std::string &word) const {
    Node *cur = root;
    for (char c : word) {
        // if word is "maz" and trie has only "ma" so that the word is not exist
        if (!cur->children[c - 'a']) return false;
        cur = cur->children[c - 'a'];
    }
    // if the word is "maz" and trie has only "mazz" so the word is not exist
    return cur->isEnd;

}

bool Trie::remove(const std::string &word) { return remove(root, word, 0); }


bool Trie::remove(Node *&node, const std::string &word, int depth) {
    // base cases
    if (!node) return false;
    if (depth == word.size()) {
        if (!node->isEnd) return false;
        node->isEnd = false;
        bool hasChild = false;
        for (int i = 0; i < 26; ++i)
            if (node->children[i]) {
                hasChild = true;
                break;
            }
        if (!hasChild) { delete node; node = nullptr; }
        return true;
    }
    char c = word[depth];
    if (!node->children[c - 'a']) return false;

    // start dp recursive call to the next node
    bool removed = remove(node->children[c - 'a'], word, depth + 1);
    if (removed && !node->children[c - 'a']) {
        bool hasChild = false;
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                hasChild = true;
                break;
            }
        }
        if (!hasChild && !node->isEnd) {
            delete node;
            node = nullptr;
        }
    }
    return removed;
}

std::vector<std::string> Trie::suggestionsDFS(const std::string &prefix, int maxResults) const {
    std::vector<std::string> out;
    Node *cur = root;
    for (char c : prefix) {
        if (!cur->children[c - 'a']) return out;
        cur = cur->children[c - 'a'];
    }
    dfs(cur, prefix, out, maxResults);
    return out;
}

std::vector<std::string> Trie::suggestionsBFS(const std::string &prefix, int maxResults) const {
    std::vector<std::string> out;
    Node *cur = root;
    for (char c : prefix) {
        if (!cur->children[c - 'a']) return out;
        cur = cur->children[c - 'a'];
    }
    std::queue<std::pair<Node*, std::string>> q;
    q.push({cur, prefix});
    while (!q.empty() && out.size() < maxResults) {
        auto [node, word] = q.front(); q.pop();
        if (node->isEnd) out.push_back(word);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                q.push({node->children[i], word + char('a' + i)});
            }
        }
    }
    return out;
}

std::vector<std::string> Trie::suggestionsFreq(const std::string &prefix, int maxResults) const {
    auto base = suggestionsDFS(prefix, 100000);
    // lambda function to compare with to frequencies
    std::sort(base.begin(), base.end(), [&](auto &a, auto &b){ return wordsCounter.getFreq(a) > wordsCounter.getFreq(b); });
    if (base.size() > maxResults) base.resize(maxResults);
    return base;
}


void Trie::dfs(Node *node, const std::string &prefix, std::vector<std::string> &out, int maxResults) const {
    if (out.size() >= maxResults) return;
    if (node->isEnd) out.push_back(prefix);
    for (int i = 0; i < 26; ++i) {
        if (node->children[i]) {
            dfs(node->children[i], prefix + char('a' + i), out, maxResults);
            if (out.size() >= maxResults) return;
        }
    }
}
