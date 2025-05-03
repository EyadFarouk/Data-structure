#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// Trie node definition
struct TrieNode {
    bool isEnd;
    TrieNode* children[26];
    TrieNode() : isEnd(false) {
        for (int i = 0; i < 26; ++i) children[i] = nullptr;
    }
};

class Trie {
public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { clear(root); }

    // Inserts a word into the trie
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) node->children[idx] = new TrieNode();
            node = node->children[idx];
        }
        node->isEnd = true;
    }

    // Primary auto-complete (DFS) alias
    vector<string> autocomplete(const string& prefix) const {
        return autocompleteDFS(prefix);
    }

    // Auto-complete using DFS directly
    vector<string> autocompleteDFS(const string& prefix) const {
        vector<string> results;
        TrieNode* node = traverse(prefix);
        if (!node) return results;
        dfs(node, prefix, results);
        return results;
    }

    // Auto-complete using BFS
    // vector<string> autocompleteBFS(const string& prefix) const {
    //     vector<string> results;
    //     TrieNode* node = traverse(prefix);
    //     if (!node) return results;

    //     queue<pair<TrieNode*, string>> q;
    //     q.push({ node, "" });
    //     while (!q.empty()) {
    //         auto [curr, suffix] = q.front(); q.pop();
    //         if (curr->isEnd) results.push_back(prefix + suffix);
    //         for (int i = 0; i < 26; ++i) {
    //             if (curr->children[i]) {
    //                 q.push({ curr->children[i], suffix + static_cast<char>('a' + i) });
    //             }
    //         }
    //     }
    //     return results;
    // }

private:
    TrieNode* root;

    // Recursively delete all nodes
    void clear(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i) clear(node->children[i]);
        delete node;
    }

    // Traverse the trie to the end of a given prefix
    TrieNode* traverse(const string& prefix) const {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) return nullptr;
            node = node->children[idx];
        }
        return node;
    }

    // DFS helper to collect all words under a node
    void dfs(TrieNode* node, const string& path, vector<string>& out) const {
        if (node->isEnd) out.push_back(path);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                dfs(node->children[i], path + static_cast<char>('a' + i), out);
            }
        }
    }
};

int main() {
    Trie trie;
    vector<string> words = {"apple", "app", "application", "apt", "bat", "ball", "batch"};
    for (const auto& w : words) trie.insert(w);

    string prefix = "app";
    cout << "Autocomplete (DFS) for prefix '" << prefix << "':\n";
    for (const auto& w : trie.autocomplete(prefix)) cout << "  " << w << "\n";

    // cout << "\nAutocomplete (BFS) for prefix '" << prefix << "':\n";
    // for (const auto& w : trie.autocompleteBFS(prefix)) cout << "  " << w << "\n";

    return 0;
}
