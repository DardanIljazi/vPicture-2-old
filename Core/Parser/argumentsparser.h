#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <QCommandLineParser>

class ArgumentsParser : public QObject, QCommandLineParser
{
    Q_OBJECT
public:
    ArgumentsParser(QCoreApplication *app);

signals:

public slots:

private:


};

#endif // ARGUMENTSPARSER_H
