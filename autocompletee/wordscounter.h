#ifndef WORDSCOUNTER_H
#define WORDSCOUNTER_H
#include <unordered_map>
#include <string>

class WordsCounter
{
public:
    static const int DEFAULT_COUNT = 3;
    WordsCounter(const WordsCounter&) = delete;
    static WordsCounter& getInstance() {
        static WordsCounter instance;
        return instance;
    }
    void incrementFreq(const std::string& word);
    int getFreq(const std::string& word);
    void setFreq(const std::string& word, int counter = DEFAULT_COUNT);
    void eraseFreq(const std::string& word);
    std::unordered_map<std::string, int> getFreqMap();


private:
    WordsCounter();
    std::unordered_map<std::string, int> freqMap;

};

#endif // WORDSCOUNTER_H
