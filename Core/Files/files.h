#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <QUrl>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeData>

#include <QDebug>

#include "../Errors/errors.h"

/*
 * This class open the files to be uploaded.
 * It remembers in a variable all the files uploaded during the software running
 */
class Files : public QObject
{
    Q_OBJECT
public:
    explicit Files(QObject *parent = 0, Errors *errors = 0);

signals:

public slots:
    /*
     * Function that fill m_urls
     */
    void setUrls(QList<QUrl> listUrls);
    /*
     * Return the file data of the listUrls
     */
    QByteArray getFileData(int at=0);

    /*
     * Called in ::getFileData to copy the QList<Qurl> into m_urls and m_listUrls
     */
    void saveUrls(QList<QUrl> listUrls);
    /*
     * Function that return a QList with all the data of the files in url m_urls, contained in m_dataFile
     */
    QList<QByteArray> getAllDataFiles();

    /*
     * Return the m_urls variable
     */
    QList<QUrl> getUrls();

    /*
     * Return a QFileInfo variable that gives informations about the file
     */
    QFileInfo getFileInfo(int at=0);

    /*
     * Return the Mime type of the url file m_urls
     */
    QString getMimeTypeFile(int at=0);


private:
    // m_listUrls is a variable that contains all the urls used during the software execution, it must not be used to do something else.
    QList<QUrl> m_listUrls;
    // m_urls contains the last urls of the files Drag And Dropped on the software
    QList<QUrl> m_urls;
    // m_dataFile contains the data of each file for m_urls
    QList<QByteArray> m_dataFile;

    Errors *localErrors;
};

#endif // FILES_H
