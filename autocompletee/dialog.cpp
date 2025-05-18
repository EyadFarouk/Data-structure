#include "dialog.h"

Dialog::Dialog() {}
QMessageBox::StandardButton Dialog::showMessage(
    QWidget* parent,
    QMessageBox::Icon icon,
    const QString& title,
    const QString& text,
    const QString& okText ,
    const QString& cancelText
    ) {
    QMessageBox msg(parent);
    msg.setIcon(icon);
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setButtonText(QMessageBox::Ok, okText);
    msg.setButtonText(QMessageBox::Cancel, cancelText);
    return static_cast<QMessageBox::StandardButton>(msg.exec());
}
bool Dialog::getText(
    QWidget* parent,
    const QString& title,
    const QString& label,
    QString& outText,
    const QString& okText,
    const QString& cancelText,
    QLineEdit::EchoMode echoMode
    ) {
    QInputDialog dlg(parent);
    dlg.setWindowTitle(title);
    dlg.setLabelText(label);
    dlg.setTextValue(outText);
    dlg.setOkButtonText(okText);
    dlg.setCancelButtonText(cancelText);
    dlg.setTextEchoMode(echoMode);
    if (dlg.exec() == QDialog::Accepted) {
        outText = dlg.textValue();
        return true;
    }
    return false;
}
