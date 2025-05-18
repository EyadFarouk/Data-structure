#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

FileManager::FileManager() {

}
void FileManager::load() {
    QFile file("dictionary.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    QString line;
    while (in.readLineInto(&line)) {
        auto parts = line.trimmed().split(QRegularExpression("\\s+"));
        if (parts.size() >= 1) {
            QString w = parts[0].toLower();
            int freq = (parts.size() >= 2) ? parts[1].toInt() : 0;
            if(freq >= WordsCounter::DEFAULT_COUNT) trie.insert(w.toStdString());
            wordsCounter.setFreq(w.toStdString(), freq);
        }
    }
}

void FileManager::save() {
    QFile file("dictionary.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    for (const auto &i : wordsCounter.getFreqMap())
        out << QString::fromStdString(i.first) << " " << i.second << "\n";
}
