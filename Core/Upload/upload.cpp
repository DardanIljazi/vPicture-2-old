#include "upload.h"

Upload::Upload(QObject *parent, Errors *errors, Files *files, Network *network, Url *url) :
    QObject(parent)
{
    localFiles   = files;
    localNetwork = network;
    localUrl     = url;
}

void Upload::upload(QList<QUrl> listUrls){
    QByteArray data;

    for(int i = 0; i < listUrls.count(); i ++)
    {
        qDebug() << "Upload::upload  localFiles->getMimeTypeFile(): " << localFiles->getMimeTypeFile(i) << " localFiles->localFiles->getFileInfo().fileName(): " << localFiles->getFileInfo(i).fileName() << endl;
        data.append(QString("\r\n--" + QString(REQUEST_BOUNDARY) + "\r\n").toLatin1());
        data.append(QString("Content-Disposition: form-data; name=\"img[]\"; filename=\"%1\"\r\n").arg(localFiles->getFileInfo(i).fileName()));
        data.append(QString("Content-Type: %1\r\n\r\n").arg(localFiles->getMimeTypeFile(i)));
        data.append(localFiles->getFileData(i));
    }
    data.append(QString("\r\n--" + QString(REQUEST_BOUNDARY) + "--\r\n"));


    // We want to use the sendimages function in the php file
    localUrl->url("sendimages");
    // We send file
    localNetwork->postFile(localUrl->getUrl(), data);

}

void Upload::uploadPixmap(QPixmap pixmap){
    QByteArray data, fileData;
    QBuffer buffer(&fileData);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    data.append(QString("\r\n--" + QString(REQUEST_BOUNDARY) + "\r\n").toLatin1());
    data.append(QString("Content-Disposition: form-data; name=\"img[]\"; filename=\"%1\"\r\n").arg("screenshotvPicture.png"));
    data.append(QString("Content-Type: %1\r\n\r\n").arg("image/png"));
    data.append(fileData);
    data.append(QString("\r\n--" + QString(REQUEST_BOUNDARY) + "--\r\n"));

    localUrl->url("sendimages");

    localNetwork->postFile(localUrl->getUrl(), data);
}
