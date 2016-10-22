#include "Thread.h"

Thread::Thread()
{
    localConfig = new Config;
}

void Thread::run()
{

}

void Thread::initListUrls(QStringList *listPathImages, QListWidget *listWidget, QSignalMapper *m_signalMapper,
                          QList<QPushButton*> *linksButtons, QList<QPushButton*> *copyButtons)
{

}

void Thread::deleteListDuplicates(QStringList currentLinksInList, QStringList *links){

}

QString Thread::getHttpLocalFilePath(QString httpLink){
    return QString("");
}
