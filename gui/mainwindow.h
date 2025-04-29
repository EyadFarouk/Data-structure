#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_newFolder_clicked();
    void on_openFolder_clicked();
    void onFolderSelected(const QModelIndex &index);
    void onNewNoteClicked();
    void onNoteSelected(const QModelIndex &index);
    void on_saveNote_clicked();
    void updateMarkdownPreview();
private:
    Ui::MainWindow *ui;
    QFileSystemModel *fsModel;
    QFileSystemModel *notesModel;
    QString currentNotesFolder;
    QString currentNoteFile;
    void loadFolder(QString folder = QString());
    void promptForFolder();

};
#endif // MAINWINDOW_H
