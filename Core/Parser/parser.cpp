#include "parser.h"

Parser::Parser(QObject *parent, ServerData *serverData, Settings *settings) :
    QObject(parent)
{
    localServerData = serverData;
    localSettings   = settings;
}

/*
 * Parser::parse is the function that receive all data from server.
 * Server data is encoded into json language
 */

void Parser::parse(QByteArray data){
    m_jsonDocument = QJsonDocument::fromJson(data);
    m_jsonObject = m_jsonDocument.object();
    m_jsonMap = m_jsonObject.toVariantMap();

    localServerData->setData(m_jsonMap);

    qDebug() << "Parser::parse: " << endl << data << endl;

    /******** PARSER ********/
    /*
     * After connection by the client
     */
    if(localServerData->getValue("action").toString() == "connect"){
        emit clientConnected();
    }
    /*
     * When the url of the uploaded picture is returned
     */
    else if(localServerData->getValue("action").toString() == "sendimages"){
        QVariantMap img;
        img.insert("url", localServerData->getValue("url").toString());
        img.insert("directimg", localServerData->getValue("directimg").toString());
        emit urlReceived(img);
    }
    /*
     * When the urls of getimagewithdate are sent
     */
    else if(localServerData->getValue("action").toString() == "getimageswithdate"){
        qDebug() << localServerData->getValue("uploads").toStringList() << endl;
        if(localServerData->getValue("uploads").toStringList().isEmpty()){
            QStringList list;
            list << "empty";
            emit urlImagesReceived(list);
        }
        else
            emit urlImagesReceived(localServerData->getValue("uploads").toStringList());
    }
    /************************/
    /*
     * When the last version of software is sent
     */
    else if(localServerData->getValue("action").toString() == "checkforupdates"){
        qDebug() << localServerData->getValue("actualVersion").toString() << endl;
        emit versionReceived(localServerData->getValue("actualVersion").toString());
    }
    /************************/

}

