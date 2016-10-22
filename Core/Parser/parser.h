#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "../ServerData/serverdata.h"
#include "../Config/settings.h"

#include <QDebug>



class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0, ServerData *serverData = 0, Settings *settings = 0);

signals:
    /*
     * Function called when the the action key (in json language) is "connect" in the server response
     */
    void clientConnected();
    /*
     * Function called when the url of the picture (or folder) is returned by the server
     */
    void urlReceived(QVariantMap img);
    /*
     * Function called when the last urls for getimageswithdate are returned by server
     */
    void urlImagesReceived(QStringList picturesLink);
    /*
     * Function called when the last version of vPicture is returned by server
     */
    void versionReceived(QString version);

public slots:
    void parse(QByteArray data);

private:
    QJsonDocument m_jsonDocument;
    QJsonObject   m_jsonObject;
    QVariantMap   m_jsonMap;

    ServerData    *localServerData;
    Settings      *localSettings;
};

#endif // PARSER_H
