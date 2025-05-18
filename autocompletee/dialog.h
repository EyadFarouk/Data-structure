#ifndef DIALOG_H
#define DIALOG_H
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QWidget>
class Dialog
{
public:
    Dialog();
    static QMessageBox::StandardButton showMessage(
        QWidget* parent,
        QMessageBox::Icon icon,
        const QString& title,
        const QString& text,
        const QString& okText = QObject::tr("OK"),
        const QString& cancelText = QObject::tr("Cancel")
        );
    static bool getText(
        QWidget* parent,
        const QString& title,
        const QString& label,
        QString& outText,
        const QString& okText = QObject::tr("OK"),
        const QString& cancelText = QObject::tr("Cancel"),
        QLineEdit::EchoMode echoMode = QLineEdit::Normal
        );
};

#endif // DIALOG_H
