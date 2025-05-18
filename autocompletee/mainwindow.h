#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <unordered_map>
#include "trie.h"
#include "regexmanager.h"
#include "wordscounter.h"
#include "filemanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTextChanged(const QString &text);
    void onReturnPressed();
    bool eventFilter(QObject* obj, QEvent* ev);
    void onAddWord();
    void onDeleteWord();
    void onItemClicked(QListWidgetItem *item);
    void onModeSelected(int mode);

private:
    // UI elements
    QLineEdit *input;
    QListWidget *list;
    QPlainTextEdit *preview;
    QComboBox *orderCombo;
    QPushButton *addButton;
    QPushButton *deleteButton;

    // Setup helpers
    void initWidgets();
    void initLayouts();
    void initConnections();
    void applyWindowFlags();

    // logic
    Trie& trie = Trie::getInstance();
    WordsCounter& wordsCounter = WordsCounter::getInstance();
    std::unordered_map<std::string, int> freqMap;
    std::unordered_map<std::string, int> newCounts;
    FileManager& fileManager = FileManager::getInstance();
    std::vector<std::string> orderSuggestions(const std::string &prefix);
    RegexManager regexManager;

    // gui functions
    void updateList(const QString& originalPrefix, int mode);
};

#endif
