#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fsModel(new QFileSystemModel(this))
{
    ui->setupUi(this);

    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::onFolderSelected);
    connect(ui->newNote, &QPushButton::clicked, this, &MainWindow::onNewNoteClicked);
    connect(ui->treeViewNotes, &QTreeView::clicked, this, &MainWindow::onNoteSelected);
    connect(ui->plainTextEditNote, &QPlainTextEdit::textChanged, this, &MainWindow::updateMarkdownPreview);
     ui->previewNote->setStyleSheet("QTextEdit { padding: 15px; }");
    QSettings settings("MyCompany", "MyApp");
    QString saved = settings.value("lastFolder").toString();

    if (saved.isEmpty() || !QDir(saved).exists()) {
        promptForFolder(); // shows the file dialog
    } else {
        loadFolder(saved); // directly load the folder
    }
    notesModel = new QFileSystemModel(this);
    notesModel->setNameFilters(QStringList() << "*.md");
    notesModel->setNameFilterDisables(false);
    notesModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    // Attach model to tree
    ui->treeViewNotes->setModel(notesModel);

    // DO NOT setRootIndex to anything yet
    // Let it remain without any index
    // Set a dummy root path (so model loads nothing internally)
    QString dummyPath = QDir::tempPath() + "/__empty__";  // temp folder unlikely to contain .md files
    QDir().mkdir(dummyPath); // Make sure it exists

    notesModel->setRootPath(dummyPath);
    ui->treeViewNotes->setRootIndex(notesModel->index(dummyPath));


}

void MainWindow::on_newFolder_clicked()
{
    // 1) Grab the current index in the tree
    QModelIndex idx = ui->treeView->currentIndex();
    if (!idx.isValid()) {
        // nothing selected — fall back to the root path
        idx = fsModel->index(fsModel->rootPath());
    }

    // 2) If they’ve selected a *file*, climb up to its parent folder
    if (!fsModel->isDir(idx)) {
        idx = idx.parent();
    }

    // 3) Ask the user for the new folder’s name
    bool ok;
    QString name = QInputDialog::getText(this,
                                         tr("Create New Folder"),
                                         tr("Folder name:"),
                                         QLineEdit::Normal,
                                         QString(), &ok);
    if (!ok || name.isEmpty())
        return;

    // 4) Create it under the selected folder
    QModelIndex newIdx = fsModel->mkdir(idx, name);

    // 5) Expand the parent so they can see it, then select the new folder
    ui->treeView->expand(idx);
    if (newIdx.isValid()) {
        ui->treeView->setCurrentIndex(newIdx);
        ui->treeView->scrollTo(newIdx);
    }
}


void MainWindow::on_openFolder_clicked()
{
    promptForFolder();
}

void MainWindow::loadFolder(QString folder)
{
    if (folder.isEmpty() || !QDir(folder).exists())
        return;

    fsModel->setRootPath(folder);
    fsModel->setReadOnly(false);
    fsModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(fsModel);
    ui->treeView->setRootIndex(fsModel->index(folder));
}

void MainWindow::promptForFolder()
{
    QString folder = QFileDialog::getExistingDirectory(
        this,
        tr("Select a folder to open"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!folder.isEmpty()) {
        QSettings settings("MyCompany", "MyApp");
        settings.setValue("lastFolder", folder);
        loadFolder(folder);
    }
}

void MainWindow::onFolderSelected(const QModelIndex &index)
{
    QString selectedFolder = fsModel->filePath(index);

    if (QDir(selectedFolder).exists()) {
        currentNotesFolder = selectedFolder;

        notesModel->setRootPath(selectedFolder);
        ui->treeViewNotes->setRootIndex(notesModel->index(selectedFolder));


    }
}

void MainWindow::onNewNoteClicked()
{
    // Check if a folder is selected
    if (currentNotesFolder.isEmpty()) {
        QMessageBox::warning(this, "No Folder Selected", "Please select a folder from the folder tree first.");
        return;
    }

    // Prompt the user for the new note's name (without extension)
    bool ok;
    QString noteName = QInputDialog::getText(this, "New Note", "Enter note name (without extension):", QLineEdit::Normal, "", &ok);

    if (ok && !noteName.isEmpty()) {
        // Create the new file path (with .md extension)
        QString filePath = QDir(currentNotesFolder).filePath(noteName + ".md");

        // Check if file already exists
        QFile file(filePath);
        if (file.exists()) {
            QMessageBox::warning(this, "File Exists", "A note with that name already exists.");
            return;
        }

        // Create and open the file (write empty content to start)
        if (file.open(QIODevice::WriteOnly)) {
            // Optionally write some default content (like title)
            QTextStream out(&file);
            out << "# " << noteName << "\n";  // Default content for the note
            file.close();

            // Refresh the notes tree to show the new file
            notesModel->setRootPath(currentNotesFolder);
            ui->treeViewNotes->setRootIndex(notesModel->index(currentNotesFolder));


        }
    }
}

void MainWindow::onNoteSelected(const QModelIndex &index)
{
    QString filePath = notesModel->filePath(index);

    if (filePath.endsWith(".md", Qt::CaseInsensitive)) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            file.close();

            ui->plainTextEditNote->setPlainText(content);
            updateMarkdownPreview(); // Show preview
            currentNoteFile = filePath;
        }
    }
}

void MainWindow::on_saveNote_clicked()
{
    if (!currentNoteFile.isEmpty()) {
        QFile file(currentNoteFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->plainTextEditNote->toPlainText();
            file.close();
        }
    }
}


void MainWindow::updateMarkdownPreview()
{
    QString markdown = ui->plainTextEditNote->toPlainText();

    // Use setMarkdown if your Qt version supports it
    ui->previewNote->setMarkdown(markdown);

    // OR use a converter if needed
}

MainWindow::~MainWindow()
{
    delete ui;
}
