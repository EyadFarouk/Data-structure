#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QString>
#include <QRegularExpression>

class ErrorHandler {
public:
    static bool validateWord(const QString& word, QString& errorMessage);
    
private:
    static bool checkEmpty(const QString& word, QString& errorMessage);
    static bool checkFormat(const QString& word, QString& errorMessage);
    static bool checkLength(const QString& word, QString& errorMessage);
    static bool checkSpecialCharacters(const QString& word, QString& errorMessage);
    
    static const int MIN_WORD_LENGTH = 1;
    static const int MAX_WORD_LENGTH = 50;
};

#endif // ERRORHANDLER_H
