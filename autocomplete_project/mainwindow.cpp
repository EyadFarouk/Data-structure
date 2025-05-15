#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegularExpression>
#include <QStringList>
#include <QListWidgetItem>
#include <QLabel>
#include <QFont>
#include <algorithm>
#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include "backgroundwidget.h"
#include "errorhandler.h"

static QString toLower(const QString &s) { return s.toLower(); }
static QString matchCase(const QString &pattern, const QString &word) {
    QString result = word;
    int n = std::min(pattern.size(), word.size());
    for (int i = 0; i < n; ++i) {
        if (pattern[i].isUpper())
            result[i] = word[i].toUpper();
        else
            result[i] = word[i].toLower();
    }
    return result;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    applyWindowFlags();
    initWidgets();
    initLayouts();
    initConnections();
    loadDictionary();
}

MainWindow::~MainWindow() {
    saveDictionary();
}

void MainWindow::loadDictionary() {


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

void MainWindow::saveDictionary() const {
    QFile file("dictionary.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    auto words = trie.suggestionsDFS("", 100000);
    for (const auto &w : words)
        out << QString::fromStdString(w) << " " << wordsCounter.getFreq(w) << "\n";

}

/// takes prefix and pass it to trie with the order selected by user
std::vector<std::string> MainWindow::orderSuggestions(const std::string &prefix) {
    int mode = orderCombo->currentIndex();
    return (!mode ? trie.suggestionsFreq(prefix, 50) :
                ( mode == 1 ? trie.suggestionsBFS(prefix, 50) : trie.suggestionsDFS(prefix, 50)));
}

/// when user type list get updated dynamically
void MainWindow::onTextChanged(const QString &text) {
    list->clear();
    QString prefix = toLower(text.trimmed());
    if (prefix.isEmpty()) return;

    std::vector<std::string> results;
    if (prefix.contains('*') || prefix.contains('%'))
        results = regexManager.getWords(prefix, orderCombo->currentIndex());
    else results = orderSuggestions(prefix.toStdString());
    for (const auto &w : results) {
        QString display = QString("%1").arg(QString::fromStdString(w));
        QListWidgetItem *item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, QString::fromStdString(w));
        list->addItem(item);
    }
}

/// when user select item from list the word send to preview and clear the list
void MainWindow::onItemClicked(QListWidgetItem *item) {
    QString prefix = input->text();
    QString rawWord = item->data(Qt::UserRole).toString();
    QString cased = matchCase(prefix, rawWord);

    preview->insertPlainText(cased + " ");
    wordsCounter.incrementFreq(rawWord.toStdString());
    input->clear();
    list->clear();

}

/// when user pressed Enter (or Space) the word in input is send to preview
void MainWindow::onReturnPressed() {
    QString word = toLower(input->text().trimmed());
    if (word.isEmpty()) return;
    std::string w = word.toStdString();
    if (!trie.contains(w)) {
        wordsCounter.incrementCount(w);
        if (wordsCounter.getCount(w) >= 3) {
            trie.insert(w);
            wordsCounter.setFreq(w);
            QMessageBox::information(this, "Added", word + " added to dictionary.");
            wordsCounter.resetCount(w);
        }
    }
    preview->insertPlainText(input->displayText() + " ");
    input->clear();
    list->clear();
}

void MainWindow::onAddWord() {
    QString word = QInputDialog::getText(this, "Add Word", "Enter new word:").trimmed();
    
    QString errorMessage;
    if (!ErrorHandler::validateWord(word, errorMessage)) {
        QMessageBox::warning(this, "Invalid Input", errorMessage);
        return;
    }
    
    word = word.toLower();
    if (trie.contains(word.toStdString())) {
        QMessageBox::information(this, "Exists", "Word already in dictionary.");
        return;
    }
    
    trie.insert(word.toStdString());
    wordsCounter.setFreq(word.toStdString(), 0);
    QMessageBox::information(this, "Added", word + " added successfully to the dictionary.");
}

void MainWindow::onDeleteWord() {
    QString prefix = QInputDialog::getText(this, "Delete Word", "Enter word or prefix to delete:").trimmed().toLower();
    if (prefix.isEmpty()) return;
    auto matches = trie.suggestionsDFS(prefix.toStdString(), 50);
    if (matches.empty()) {
        QMessageBox::information(this, "None", "No matches found.");
        return;
    }
    QStringList items;
    for (const auto &m : matches) items << QString::fromStdString(m);
    QString choice = QInputDialog::getItem(this, "Select", "Choose word to delete:", items, 0, false);
    if (choice.isEmpty()) return;
    if (trie.remove(choice.toStdString())) {
        QMessageBox::information(this, "Deleted", choice + " removed.");
        wordsCounter.eraseFreq(choice.toStdString());
    } else {
        QMessageBox::warning(this, "Error", "Could not delete.");
    }
}
void MainWindow::onModeSelected(int mode) {
    list->clear();
    QString prefix = toLower(input->text());
    if (prefix.isEmpty()) return;

    std::vector<std::string> results;
    if (prefix.contains('*') || prefix.contains('%'))
        results = regexManager.getWords(prefix, orderCombo->currentIndex());
    else results = orderSuggestions(prefix.toStdString());
    for (const auto &w : results) {
        QString display = QString("%1").arg(QString::fromStdString(w));
        QListWidgetItem *item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, QString::fromStdString(w));
        list->addItem(item);
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == input && ev->type() == QEvent::KeyPress) {
        auto* kev = static_cast<QKeyEvent*>(ev);
        if (kev->key() == Qt::Key_Space || kev->key() == Qt::Key_Return) {
            onReturnPressed();
            return true;
    }
    }
    return QMainWindow::eventFilter(obj, ev);
}

void MainWindow::applyWindowFlags() {
    setWindowState(Qt::WindowMaximized);
    setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::initWidgets() {
    // central widget with background
    auto* central = new BackgroundWidget(":/images/bg.jpg", this);
    setCentralWidget(central);

    // title
    auto* title = new QLabel(tr("كملني"), this);
    title->setAlignment(Qt::AlignCenter);
    QFont f = title->font(); f.setPointSize(24); f.setBold(true);
    title->setFont(f);

    // input & preview
    input       = new QLineEdit;
    list        = new QListWidget;
    preview     = new QPlainTextEdit;
    orderCombo  = new QComboBox;
    addButton   = new QPushButton(tr("Add Word"));
    deleteButton= new QPushButton(tr("Delete Word"));

    input->setPlaceholderText(tr("Start typing..."));
    list->setWordWrap(true);

    orderCombo->addItems({
        tr("Frequency"), tr("Length"), tr("Lexicographical")
    });
}

void MainWindow::initLayouts() {
    auto* central = centralWidget();
    auto* vlay = new QVBoxLayout(central);
    vlay->setContentsMargins(20,20,20,20);
    vlay->setSpacing(15);

    // Title
    vlay->addWidget(findChild<QLabel*>());

    // Input row
    vlay->addWidget(input);

    // Controls row
    auto* ctrl = new QHBoxLayout;
    ctrl->addWidget(orderCombo);
    ctrl->addWidget(addButton);
    ctrl->addWidget(deleteButton);
    vlay->addLayout(ctrl);

    // Main split
    auto* split = new QHBoxLayout;
    split->addWidget(list);
    split->addWidget(preview);
    vlay->addLayout(split);
}

void MainWindow::initConnections() {
    input->installEventFilter(this);
    connect(input, &QLineEdit::textChanged,
            this, &MainWindow::onTextChanged);
    connect(input, &QLineEdit::returnPressed,
            this, &MainWindow::onReturnPressed);
    connect(addButton, &QPushButton::clicked,
            this, &MainWindow::onAddWord);
    connect(deleteButton, &QPushButton::clicked,
            this, &MainWindow::onDeleteWord);
    connect(list, &QListWidget::itemClicked,
            this, &MainWindow::onItemClicked);
    connect(orderCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,&MainWindow::onModeSelected);
}
