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
    for (auto &p : node->children) clear(p.second);

    // delete node that (is end) or its children have been deleted
    delete node;
}

// Complixity -> O(n) n is the length of the word
void Trie::insert(const std::string &word) {
    Node *cur = root;
    for (char c : word) {
        /*
         * count function in unorderd map return the count of the key without creating new object
        */
        if (!cur->children.count(c)) cur->children[c] = new Node(); // make new trie from this letter
        cur = cur->children[c];
    }
    cur->isEnd = true;
}


bool Trie::contains(const std::string &word) const {
    Node *cur = root;
    for (char c : word) {
        // if word is "maz" and trie has only "ma" so that the word is not exist
        if (!cur->children.count(c)) return false;
        cur = cur->children.at(c);
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
        if (node->children.empty()) { delete node; node = nullptr; }
        return true;
    }
    char c = word[depth]; // maz
    if (!node->children.count(c)) return false;

    // start dp recursive call to the next node
    bool removed = remove(node->children[c], word, depth + 1);
    if (removed && node->children[c] == nullptr) node->children.erase(c);
    return removed;
}

std::vector<std::string> Trie::suggestionsDFS(const std::string &prefix, int maxResults) const {
    std::vector<std::string> out;
    Node *cur = root;
    for (char c : prefix) {
        if (!cur->children.count(c)) return out;
        cur = cur->children.at(c);
    }
    dfs(cur, prefix, out, maxResults);
    return out;
}

std::vector<std::string> Trie::suggestionsBFS(const std::string &prefix, int maxResults) const {
    std::vector<std::string> out;
    Node *cur = root;
    for (char c : prefix) {
        if (!cur->children.count(c)) return out;
        cur = cur->children.at(c);
    }
    std::queue<std::pair<Node*, std::string>> q;
    q.push({cur, prefix});
    while (!q.empty() && out.size() < maxResults) {
        auto [node, word] = q.front(); q.pop();
        if (node->isEnd) out.push_back(word);
        for (auto &p : node->children) q.push({p.second, word + p.first});
    }
    return out;
}

std::vector<std::string> Trie::suggestionsFreq(const std::string &prefix, int maxResults) const {
    auto base = suggestionsDFS(prefix, 100000);
    // lambda function to compare with to frequencies
    std::sort(base.begin(), base.end(), [&](auto &a, auto &b){ return wordsCounter.getFreq(a) > wordsCounter.getFreq(b); });
    if (base.size() > 50) base.resize(50); // return only 50 result
    return base;
}


void Trie::dfs(Node *node, const std::string &prefix, std::vector<std::string> &out, int maxResults) const {
    if (out.size() >= maxResults) return;
    if (node->isEnd) out.push_back(prefix);
    for (auto &p : node->children) {
        dfs(p.second, prefix + p.first, out, maxResults);
        if (out.size() >= maxResults) return;
    }
}
