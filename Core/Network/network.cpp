#include "network.h"

Network::Network(QObject *parent, Errors *errors, Parser *parser, Url *url, Settings *settings, ServerData *serverData) :
    QObject(parent)
{
    m_accessManager = new QNetworkAccessManager(this);
    cookieJar = new CookieJar(this);
    m_accessManager->setCookieJar(cookieJar);

    localErrors     = errors;
    localParser     = parser;
    localUrl        = url;
    localSettings   = settings;
    localServerData = serverData;
    secure          = new Secure;
}

void Network::goRequest(int type, QByteArray data, int typeExtern){
    if(type == Network::Get)
        m_reply = m_accessManager->get(m_request);
    else if(type == Network::Post || type == Network::Post_File)
        m_reply = m_accessManager->post(m_request, data);

    if(type == Network::Post_File){
        QObject::connect(m_reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgression(qint64,qint64)) );
    }

    if(typeExtern == Network::Download_Update)
        QObject::connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(upload_update(qint64,qint64)));


    if(typeExtern == Network::Download_Update){
        QObject::connect(m_reply, SIGNAL(finished()),
                         this, SLOT(downloadFinished()));
    }else{
        QObject::connect(m_reply, SIGNAL(readyRead()),
                         this, SLOT(replyFinished()));
    }
}

void Network::replyFinished(){
    QNetworkReply *response = qobject_cast<QNetworkReply*>(sender());
    replyErrors(response);

    // Set the cookies received from server to m_request
    if(cookieJar->getAllCookies().count() > 0)
        setCookies(cookieJar->getAllCookies());

    localParser->parse(response->readAll());
    response->deleteLater();
}

void Network::downloadFinished(){
    QNetworkReply *response = qobject_cast<QNetworkReply*>(sender());
    replyErrors(response);

    emit setupReceived(response->readAll());
}

void Network::replyErrors(QNetworkReply *response){
    if(response->error() != QNetworkReply::NoError)
        local
}

void Network::setUseSsl(bool enable){

    QSslConfiguration config;
    if(enable == true){
        config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1SslV3);

    }else{
        config = QSslConfiguration::defaultConfiguration();
    }

    m_request.setSslConfiguration(config);
}

void Network::setRawHeaders(const QByteArray &headerName, const QByteArray &headerValue){
    m_request.setRawHeader(headerName, headerValue);
}

void Network::uploadProgression(qint64 bytesReceived, qint64 bytesTotal){
    emit upload(bytesReceived, bytesTotal);
}
