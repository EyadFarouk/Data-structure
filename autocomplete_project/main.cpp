#include <QApplication>
#include "mainwindow.h"
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
     QFile file(":/style.qss");     // if you load from the working directory

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Could not open style.qss";
    } else {
        QString qss = QLatin1String(file.readAll());
        a.setStyleSheet(qss);
    }
    MainWindow w;
    w.showMaximized();
    w.show();
    return a.exec();
}
