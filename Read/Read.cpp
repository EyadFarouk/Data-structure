#include "Read.h"
#include "data_structures/Trie.h"
#include <cctype>
#include <string>
#include <unordered_map>
#include <conio.h>

using namespace std;
extern unordered_map<string, int> words;
extern Trie trie;

void read()
{
    char chara;
    string word;
    do {
        chara = getch();
        if (isspace(chara))
            break;
        if (chara == '\b' && !word.empty()) {
            word.pop_back();
            cout << "\b \b";
        }
        else {
            word += chara;
            cout << chara;
        }
    }
    if (!word.empty())
        if (words.find(word) == words.end())
            words.insert(pair(word, 1));
		else {
			words[word]++;
			if (words[word] == 3)
				trie.insert(word);
		}

}
