#include "files.h"

Files::Files(QObject *parent, Errors *errors) :
    QObject(parent)
{
    localErrors = errors;
}

void Files::setUrls(QList<QUrl> listUrls){
    saveUrls(listUrls);
}

QList<QByteArray> Files::getAllDataFiles(){
    m_dataFile.clear();

    if(m_urls.count() > 0){
        for(int i = 0; i < m_urls.count(); i++){
            m_dataFile << getFileData(i);
        }
    }else{
        local
    }

    return m_dataFile;
}

QByteArray Files::getFileData(int at){
    QByteArray tmpData = 0;

    // If the file we asked to be read doesn't exist in the listUrls list
    if(at > m_urls.size()){
        local
    }else{
        QFile file(m_urls.at(at).toLocalFile());
        qDebug() << "Files::getFileData ==> file.fileName() = " << file.fileName() << endl;
        file.open(QIODevice::ReadOnly);
        tmpData = file.readAll();
        file.close();

        return tmpData;
    }

    return tmpData;
}

QFileInfo Files::getFileInfo(int at){
    return QFileInfo(m_urls.at(at).toLocalFile());
}

QString Files::getMimeTypeFile(int at){
    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(m_urls.at(at).toLocalFile());

    return type.name();
}

void Files::saveUrls(QList<QUrl> listUrls){
    m_urls = listUrls;

    // We "append" the urls. m_listUrls contains all the urls used during the software running.
    m_listUrls << listUrls;
}

QList<QUrl> Files::getUrls(){
    return m_urls;
}
