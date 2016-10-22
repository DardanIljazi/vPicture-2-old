#include "errors.h"

Errors::Errors(QObject *parent) :
    QObject(parent)
{
}

void Errors::error(int error, QString errorString, QString function, bool showMessageBox){
    m_error = error;
    m_errorString = errorString;
    m_function = function;

    // Save the above informations in error log file
    writeInFile();

    if(showMessageBox){
        showMessageBoxError();
    }
}

void Errors::showMessageBoxError(){
    Dialog *dialog = new Dialog(0, "vPicture", QString(space() + m_errorString + space(3) + m_function));
    dialog->raise();
    dialog->show();
}

void Errors::writeInFile(){
    m_lastError = QString(getDatetime() + space(2) + QString::number(m_error) + space() + m_errorString + space(3) + m_function + endLine());

    QFile file(ERROR_LOG_FILENAME);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(m_lastError.toLatin1());
    file.close();
}

QString Errors::getDatetime(){
    return QString("[" + QDateTime::currentDateTime().toString() + "]");
}

QString Errors::endLine(){
    return "\r\n"; // End of line (make a enter return)
}

QString Errors::space(int i){
    QString space;
    if(i <= 0)
        i = 1;

    for(int j = 0; j < i; j ++){
        space += " ";
    }
    return space;
}

QString Errors::getLastError(){
    return m_errorString;
}
