#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>
#include <QBuffer>
#include "../Errors/errors.h"
#include "../Network/network.h"
#include "../Files/files.h"
#include "../Url/url.h"

#define REQUEST_BOUNDARY "vPicture"

class Upload : public QObject
{
    Q_OBJECT
public:
    explicit Upload(QObject *parent = 0, Errors *errors = 0, Files *files = 0, Network *network = 0, Url *url = 0);

signals:

public slots:
    void upload(QList<QUrl> listUrls);
    void uploadPixmap(QPixmap pixmap);

private:
    Files   *localFiles;
    Network *localNetwork;
    Url     *localUrl;
};

#endif // UPLOAD_H
