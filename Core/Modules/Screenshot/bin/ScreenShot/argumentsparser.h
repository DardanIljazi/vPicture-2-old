#ifndef ARGUMENTSPARSER_H
#define ARGUMENTSPARSER_H

#include <QObject>
#include <QStringList>

class ArgumentsParser : public QObject
{
    Q_OBJECT
public:
    explicit ArgumentsParser(QStringList arguments = QStringList());

signals:

public slots:
    bool parseArguments();

private:
    QStringList lastArguments;
};

#endif // ARGUMENTSPARSER_H
