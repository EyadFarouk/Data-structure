#ifndef WORDSCOUNTER_H
#define WORDSCOUNTER_H
#include <unordered_map>
#include <string>

class WordsCounter
{
public:
    WordsCounter(const WordsCounter&) = delete;
    static WordsCounter& getInstance() {
        static WordsCounter instance;
        return instance;
    }
    void incrementCount(const std::string& word);
    void incrementFreq(const std::string& word);
    int getCount(const std::string& word);
    int getFreq(const std::string& word);
    void resetCount(const std::string& word);
    void setFreq(const std::string& word, int count = 1);
    void eraseFreq(const std::string& word);
private:
    WordsCounter();
    std::unordered_map<std::string, int> freqMap;
    std::unordered_map<std::string, int> newCounts;
};

#endif // WORDSCOUNTER_H
