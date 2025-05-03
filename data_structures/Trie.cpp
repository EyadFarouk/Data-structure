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
Trie::TrieNode *Trie::traverse(const std::string &prefix) const {
    TrieNode* node = root;
    for(auto& c : prefix) {
        if(!node->children[c])
            return nullptr;
        node = node->children[c];
    }
    return node;
}
void Trie::dfs(TrieNode *node, const std::string &path, std::vector<std::string> &out) const{
    if (node->isEndOfWord) out.push_back(path);
    for (int i = 0; i < 26; ++i) 
        if (node->children[i]) 
            dfs(node->children[i], path + static_cast<char>('a' + i), out);
    
}
std::vector<std::string> Trie::autocomplete(const std::string& prefix) const {
    std::vector<std::string> results;
    TrieNode* node = traverse(prefix);
    if (!node) return results;
    dfs(node, prefix, results);
    return results;
}
std::vector<std::string> Trie::autocomplete(const std::string &prefix, const bool &bfs) const {
    if(!bfs) return autocomplete(prefix);
    std::vector<std::string> results;
    TrieNode* node = traverse(prefix);
    if (!node) return results;

    std::queue<std::pair<TrieNode*, std::string>> q;
    q.push({ node, "" });
    while (!q.empty()) {
        auto curr = q.front(); q.pop();
        if (curr.first->isEndOfWord) results.push_back(prefix + curr.second);
        for (char i = 'a'; i <= 'z'; ++i) {
            if (curr.first->children[i]) {
                q.push({ curr.first->children[i], curr.second + i });
            }
        }
    }
    return results;

}
Trie::Trie()
{
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
void Trie::insert(std::string& word)
{
    TrieNode*node=root;
    for (char ch : word){
        if (!node->children[ch])
            node->children[ch]=new TrieNode();
        node=node->children[ch];
    }
    node->isEndOfWord = true;

}
