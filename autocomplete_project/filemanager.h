#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "wordscounter.h"
#include "trie.h"

class FileManager
{
public:
    FileManager(const FileManager&) = delete;
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }
    void load();
    void save();

private:
    WordsCounter& wordsCounter = WordsCounter::getInstance();
    Trie& trie = Trie::getInstance();
    FileManager();
};

#endif // FILEMANAGER_H
