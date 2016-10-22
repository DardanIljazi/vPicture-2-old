#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <QObject>
#include <QStringList>

#include "argumentsparser_global.h"

#define HideWindows   "hide"
#define ShowWindows   "show"
#define NoEnumWindows "noenumw" // Doesn't use EnumWindows function from Windows API
#define Connect       "/connect"
#define NoKeyEvent    "nokeyevent" // The SystemKeyBoardReadWrite class is not used (It's a windows class)

class ARGUMENTSPARSERSHARED_EXPORT ArgumentsParser : public QObject
{
    Q_OBJECT
public:
    explicit ArgumentsParser(QStringList arguments = QStringList());


signals:
    void hideWindows();
    void showWindows();
    void noEnumWindows();
    void connectUser(QString mail, QString password);
    void noKeyEvent();

public slots:
//    bool isWindowsHidden();
//    bool isWindowsShown();
//    bool isEnumWindowsFunctionUsed();
//    bool isConnectUserOptionUsed();
//    bool isKeyEventFunctionUsed();
private slots:
    bool parseArguments();

private:
    QStringList lastArguments;
};

#endif // ARGUMENTSPARSER_H
