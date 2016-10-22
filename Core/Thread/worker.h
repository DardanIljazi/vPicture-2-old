#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QtGui>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>

#include "../Config/config.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

public slots:
    void initListUrls(QStringList *listPathImages, QListWidget *listWidget);
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
    void doneAction();
    void addButtons(QString link, int i, QListWidgetItem *item);

private:
    Config *localConfig;


};

#endif // WORKER_H
