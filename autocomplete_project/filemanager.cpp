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
            trie.insert(w.toStdString());
            int freq = (parts.size() >= 2) ? parts[1].toInt() : 0;
            wordsCounter.setFreq(w.toStdString(), freq);
        }
    }
}

void FileManager::save() {
    QFile file("dictionary.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    auto words = trie.suggestionsDFS("", 100000);
    for (const auto &w : words)
        out << QString::fromStdString(w) << " " << wordsCounter.getFreq(w) << "\n";
}
