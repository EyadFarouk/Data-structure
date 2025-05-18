#ifndef WORDSCOUNTER_H
#define WORDSCOUNTER_H
#include <unordered_map>
#include <string>
using namespace std;

class WordsCounter
{
public:
    static const int DEFAULT_COUNT = 3;
    WordsCounter(const WordsCounter&) = delete;
    static WordsCounter& getInstance() {
        static WordsCounter instance;
        return instance;
    }
    void incrementFreq(const string& word);
    int getFreq(const string& word);
    void setFreq(const string& word, int counter = DEFAULT_COUNT);
    void eraseFreq(const string& word);
    unordered_map<string, int> getFreqMap();


private:
    WordsCounter();
    unordered_map<string, int> freqMap;

};

#endif // WORDSCOUNTER_H
