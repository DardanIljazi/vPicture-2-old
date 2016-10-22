#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

void Worker::initListUrls(QStringList *listPathImages, QListWidget *listWidget)
{
//    int limit = listPathImages->count();

//    listWidget->clear();
//    for(int i = 0, j = 0; i < limit; i++){
//        QListWidgetItem *item = new QListWidgetItem();
//        item->setWhatsThis(listPathImages->at(j));

//        listWidget->addItem(item);
//        listWidget->item(i)->setIcon(QIcon(getHttpLocalFilePath(listPathImages->at(j))));
//        listWidget->item(i)->setTextAlignment(Qt::AlignHCenter);

//        QPushButton *open = new QPushButton(listPathImages->at(j));
//        open->setCursor(Qt::PointingHandCursor);
//        open->setMaximumSize(130, 25);
//        open->setObjectName(QString("$pushbutton_link$") + listPathImages->at(j));

//        linksButtons->append(open);

//        QPushButton *copy = new QPushButton(tr("Copy link"));
//        copy->setCursor(Qt::PointingHandCursor);
//        copy->setMaximumSize(60, 25);
//        copy->setObjectName(QString("$pushbutton_copy$") + listPathImages->takeAt(0));

//        copyButtons->append(copy);

//        copy->setStyleSheet("QPushButton{"
//                            "border: 1px solid #34495e;"
//                            "min-width: 50px;"
//                            "padding: 3px;"
//                            "background-color: #2c3e50;"
//                            "color: #ecf0f1;"
//                            "font: 8pt \"Open Sans\";"
//                            "}"
//                            "QPushButton::hover{"
//                            "background-color: #34495e;"
//                            "}");

//        QHBoxLayout *layout = new QHBoxLayout();
//        layout->addWidget(open);
//        layout->addWidget(copy);
//        QWidget *widget = new QWidget();
//        widget->setLayout(layout);
//        listWidget->setItemWidget(item, widget);


//        m_signalMapper->setMapping(linksButtons->at(i), linksButtons->at(i)->objectName());
//        m_signalMapper->setMapping(copyButtons->at(i), copyButtons->at(i)->objectName());
//        QObject::connect(open, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
//        QObject::connect(copy, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
//    }
}

void Worker::deleteListDuplicates(QStringList currentLinksInList, QStringList *links){
    for(int i = 0; i < currentLinksInList.count(); i++){
        if(links->contains(currentLinksInList.at(i))){
            links->removeOne(currentLinksInList.at(i));
        }
    }
}

QString Worker::getHttpLocalFilePath(QString httpLink){
    QUrl url(httpLink);
    QFile file;

    for(int i = 0; i < localConfig->getListMimeDataAllowed().count(); i++){
        file.setFileName(QString("pictures") + url.path() + QString(".") + localConfig->getListMimeDataAllowed().at(i));
        if(file.exists())
            return file.fileName();
    }

    return ":/ListWidget/transparent_picture";
}
