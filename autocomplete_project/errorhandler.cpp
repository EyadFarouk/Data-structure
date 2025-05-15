#include "errorhandler.h"

bool ErrorHandler::validateWord(const QString& word, QString& errorMessage) {
    if (!checkEmpty(word, errorMessage)) return false;
    if (!checkFormat(word, errorMessage)) return false;
    if (!checkLength(word, errorMessage)) return false;
    if (!checkSpecialCharacters(word, errorMessage)) return false;
    return true;
}

bool ErrorHandler::checkEmpty(const QString& word, QString& errorMessage) {
    if (word.trimmed().isEmpty()) {
        errorMessage = "Word cannot be empty";
        return false;
    }
    return true;
}

bool ErrorHandler::checkFormat(const QString& word, QString& errorMessage) {
    QRegularExpression re("^[a-zA-Z]+$");
    if (!re.match(word).hasMatch()) {
        errorMessage = "Word can only contain letters";
        return false;
    }
    return true;
}

bool ErrorHandler::checkLength(const QString& word, QString& errorMessage) {
    int length = word.length();
    if (length < MIN_WORD_LENGTH || length > MAX_WORD_LENGTH) {
        errorMessage = QString("Word length must be between %1 and %2 characters")
                          .arg(MIN_WORD_LENGTH)
                          .arg(MAX_WORD_LENGTH);
        return false;
    }
    return true;
}

bool ErrorHandler::checkSpecialCharacters(const QString& word, QString& errorMessage) {
    QRegularExpression specialChars("[^a-zA-Z]");
    if (word.contains(specialChars)) {
        errorMessage = "Word contains invalid characters";
        return false;
    }
    return true;
}
