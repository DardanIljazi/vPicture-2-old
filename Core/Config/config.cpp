#include "config.h"

Config::Config(QObject *parent) :
    QObject(parent)
{
    m_listMimeData << "png" << "gif" << "bmp" << "xbm" << "xpm" << "tif" << "tiff" << "svg" << "ico" << "ieg" << "jpg" << "jpeg" << "jpe" << "jpf" << "jpx" << "jp2" << "j2c" << "j2k" << "jpc";
}

QStringList Config::getListMimeDataAllowed(){
    return m_listMimeData;
}

QString Config::mimeDataAllowedScheme(){
    QString temp;
    temp.append("Images (");
    for(int i = 0; i != m_listMimeData.count(); i ++){
        temp.append("*.");
        temp.append(m_listMimeData.at(i));
        if(i < m_listMimeData.count()-1)
            temp.append(" ");
    }
    temp.append(")");
    qDebug() << "AllowedUrlScheme: " << temp << endl;
    return temp;
}

QList<QAction *> Config::getLanguagesList(){
    QList<QAction *> test;
    return test;
}
