#include <iostream>
#include "Trie.cpp"

using namespace std;

int main() {
    Trie trie;
    int n; cin >> n;
    for(int i = 0; i < n; i++) {
        string s;
        cin >> s;
        trie.insert(s);
    }

    vector<string> ans = trie.autocomplete("z");
    for(auto& i : ans) cout << i << "\n";
}