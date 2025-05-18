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
#include "richtextdelegate.h"
#include "dialog.h"

static QString toLower(const QString &s) { return s.toLower(); }
static QString matchCase(const QString &pattern,  const QString &word) {
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
    fileManager.load();
}

MainWindow::~MainWindow() {
    fileManager.save();
}

/// takes prefix and pass it to trie with the order selected by user
std::vector<std::string> MainWindow::orderSuggestions(const std::string &prefix) {
    int mode = orderCombo->currentIndex();
    return (!mode ? trie.suggestionsFreq(prefix, 50) :
                ( mode == 1 ? trie.suggestionsBFS(prefix, 50) : trie.suggestionsDFS(prefix, 50)));
}

/// when user type list get updated dynamically
void MainWindow::onTextChanged(const QString &text) {
    updateList(text, orderCombo->currentIndex());
}

/// when user select item from list the word send to preview and clear the list
void MainWindow::onItemClicked(QListWidgetItem *item) {
    const QString prefix = input->text();
    const QString word = item->data(Qt::UserRole).toString();
    preview->insertPlainText(matchCase(prefix, word) + ' ');
    wordsCounter.incrementFreq(word.toStdString());
    input->clear();
    list->clear();

}

/// when user pressed Enter (or Space) the word in input is send to preview
void MainWindow::onReturnPressed() {
    const QString word = toLower(input->text().trimmed());
    if (word.isEmpty()) return;
    const std::string w = word.toStdString();
    wordsCounter.incrementFreq(w);
    if (wordsCounter.getFreq(w) == 3) {
        trie.insert(w);
        QMessageBox::information(this, tr("Added"), word + tr(" added to dictionary."));
    }
    preview->insertPlainText(input->displayText() + " ");
    input->clear();
    list->clear();
}

void MainWindow::onAddWord() {
    QString word;
    if (!Dialog::getText(
            this,
            tr("Add Word"),
            tr("Enter new word:"),
            word
            )) return;  // user cancelled

    word = word.trimmed();
    QString error;
    if (!ErrorHandler::validateWord(word, error)) {
        Dialog::showMessage(
            this,
            QMessageBox::Warning,
            tr("Invalid Input"),
            error,
            tr("OK"),
            tr("Cancel")
            );
        return;
    }

    word = word.toLower();
    if (trie.contains(word.toStdString())) {
        Dialog::showMessage(
            this,
            QMessageBox::Information,
            tr("Exists"),
            tr("Word already in dictionary."),
            tr("OK"),
            tr("Cancel")
            );
        return;
    }

    trie.insert(word.toStdString());
    wordsCounter.setFreq(word.toStdString());
    Dialog::showMessage(
        this,
        QMessageBox::Information,
        tr("Added"),
        word + tr(" added successfully to the dictionary."),
        tr("OK"),
        tr("Cancel")
        );
}

void MainWindow::onDeleteWord() {
    QString prefix;
    if (!Dialog::getText(
            this,
            tr("Delete Word"),
            tr("Enter word or prefix to delete:"),
            prefix
            )) return;

    prefix = prefix.trimmed().toLower();
    if (prefix.isEmpty()) return;

    auto matches = trie.suggestionsDFS(prefix.toStdString(), 50);
    if (matches.empty()) {
        Dialog::showMessage(
            this,
            QMessageBox::Information,
            tr("None"),
            tr("No matches found."),
            tr("OK"),
            tr("Cancel")
            );
        return;
    }

    QStringList items;
    for (const auto &m : matches) items << QString::fromStdString(m);

    bool ok;
    QString choice = QInputDialog::getItem(
        this,
        tr("Select"),
        tr("Choose word to delete:"),
        items,
        0,
        false,
        &ok
        );
    if (!ok || choice.isEmpty()) return;

    if (trie.remove(choice.toStdString())) {
        wordsCounter.eraseFreq(choice.toStdString());
        Dialog::showMessage(
            this,
            QMessageBox::Information,
            tr("Deleted"),
            choice + tr(" removed."),
            tr("OK"),
            tr("Cancel")
            );
    } else {
        Dialog::showMessage(
            this,
            QMessageBox::Warning,
            tr("Error"),
            tr("Could not delete."),
            tr("OK"),
            tr("Cancel")
            );
    }
}
void MainWindow::onModeSelected(int mode) {
    updateList(input->text(), mode);
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
    auto* central = new BackgroundWidget(":images/bg.jpg", this);
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
    list->setItemDelegate(new RichTextDelegate(list));
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

void MainWindow::updateList(const QString& originalPrefix, int mode) {
    list->clear();
    const QString trimmed = originalPrefix.trimmed();
    if (trimmed.isEmpty()) return;

    const QString lower = toLower(trimmed);
    std::vector<std::string> results;
    if (lower.contains(QRegularExpression("[%_*]"))) {
        results = regexManager.getWords(lower, mode);
    } else {
        results = orderSuggestions(lower.toStdString());
    }

    for (const auto& w : results) {
        const QString qw = QString::fromStdString(w);
        const QString cased = matchCase(trimmed, qw);
        const int len = trimmed.length();
        QString display;

        if (lower.contains('*')) {
            display = QString("<span style='font-weight:bold;color:white;'>%1</span>")
            .arg(cased);
        } else {
            const QString prefixPart = cased.left(len);
            const QString rest       = cased.mid(len);
            display = QString(
                          "<span style='font-weight:bold;color:#2D89EF;'>%1</span>"
                          "<span style='color:white;'>%2</span>")
                          .arg(prefixPart, rest);
        }

        auto* item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, qw);
        list->addItem(item);
    }
}
