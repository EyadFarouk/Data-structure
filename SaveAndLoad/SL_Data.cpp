#include "SL_Data.h"
#include <iostream>
#include "data_structures/Trie.h"
#include <fstream>
#include <unordered_map>
using namespace std;
unordered_map<string, int> words;

void saveData()
{
    fstream file;
    file.open("../Dictionaryy.txt", ios::out);
    if (file.is_open())
    {
        for (auto ex : words)
        {
            file << ex.first << " " << ex.second << '\n';
        }
        file.close();
    }
}

void loadData()
{
    fstream file;
    file.open("../Dictionary.txt", ios::in);
    if (file.is_open()) {
        string data;
        while (getline(file, data, ' '))
        {
            string key = data;
            getline(file, data);
            int value = stoi(data);
            words.insert({ key,value });
        }
        Trie trie;
        for (auto i : words) {
            cout << i.first << ' ' << i.second << '\n';
            if (i.second >= 3)
                trie.insert(i.first);
        }
        file.close();
    }
}