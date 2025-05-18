#include "wordscounter.h"

/*
    Word Counter class for  manage the freq and count of each word user added
*/

using namespace std;

WordsCounter::WordsCounter() {}

void WordsCounter::incrementFreq(const string& word) {
    freqMap[word]++;
}

int WordsCounter::getFreq(const string& word) {
    return freqMap[word];
}

void WordsCounter::setFreq(const string& word, int counter) {
    freqMap[word] = counter;
}

void WordsCounter::eraseFreq(const string& word) {
    freqMap.erase(word);
}
unordered_map<string, int> WordsCounter::getFreqMap() {
    return freqMap;
}
