#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtGui>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "../Config/config.h"


class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();

public slots :
    void run();
    void initListUrls(QStringList *listPathImages, QListWidget *listWidget, QSignalMapper *m_signalMapper, QList<QPushButton *> *linksButtons, QList<QPushButton *> *copyButtons);
    /*
     * Function that returns the local path of the link (http://vpictu.re/4asd)
     * This will take the name behind the last / and will try all extensions that are allowed in the Config class Config::getListMimeDataAllowed()
     */
    QString getHttpLocalFilePath(QString httpLink);
    /*
     * Function that deletes all duplicates that could be in the new links value returned
     */
    void deleteListDuplicates(QStringList currentLinksInList, QStringList *links);


signals :
    void signal();
    void actionTermine();

private:
    // Pictures
    QStringList *picturesLinks;
    QList<QPushButton*> *linksButtons;
    QList<QPushButton*> *copyButtons;
    QListWidget         *listWidget;

    QSignalMapper *m_signalMapper;

    Config *localConfig;

};


#endif // THREAD_H
