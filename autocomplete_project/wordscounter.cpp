#include "wordscounter.h"

/*
    Word Counter class for  manage the freq and count of each word user added
*/

WordsCounter::WordsCounter() {}

void WordsCounter::incrementCount(const std::string& word) {
    newCounts[word]++;
}
void WordsCounter::incrementFreq(const std::string& word) {
    freqMap[word]++;
}

int WordsCounter::getCount(const std::string& word) {
    return newCounts[word];
}
int WordsCounter::getFreq(const std::string& word) {
    return freqMap[word];
}
void WordsCounter::resetCount(const std::string& word) {
    newCounts[word] = 0;
}
void WordsCounter::setFreq(const std::string& word, int count) {
    freqMap[word] = count;
}

void WordsCounter::eraseFreq(const std::string& word) {
    freqMap.erase(word);
}
