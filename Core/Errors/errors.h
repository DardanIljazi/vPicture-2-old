#ifndef ERRORS_H
#define ERRORS_H

#include <QObject>
#include <QFile>
#include <QTime>
#include <QMessageBox>
#include "../Dialog/dialog.h"

#define ERROR_LOG_FILENAME "errors.log"


class Errors : public QObject
{
    Q_OBJECT
public:
    explicit Errors(QObject *parent = 0);

signals:

public slots:
    /*
     * Called when an error occured in a function
     */
    void error(int error, QString errorString, QString function, bool showMessageBox=false);
    /*
     * Show the error in a QMessageBox
     */
    void showMessageBoxError();

    /*
     * Write the log in the file
     */
    void writeInFile();
    /*
     * Datetime for each line in error log file
     */
    QString getDatetime();

    /*
     * Get the last error written in the log file
     */
    QString getLastError();

    QString space(int i=1);
    QString endLine();
private:
    int m_error;
    QString m_errorString, m_function;
    QString m_lastError; // This contain all the data like the datetime of the error and m_errorString
};

#endif // ERRORS_H
