#include "wordscounter.h"

/*
    Word Counter class for  manage the freq and count of each word user added
*/

WordsCounter::WordsCounter() {}

void WordsCounter::incrementFreq(const std::string& word) {
    freqMap[word]++;
}

int WordsCounter::getFreq(const std::string& word) {
    return freqMap[word];
}

void WordsCounter::setFreq(const std::string& word, int counter) {
    freqMap[word] = counter;
}

void WordsCounter::eraseFreq(const std::string& word) {
    freqMap.erase(word);
}
std::unordered_map<std::string, int> WordsCounter::getFreqMap() {
    return freqMap;
}
