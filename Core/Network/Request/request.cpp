#include "../network.h"

void Network::get(QUrl url, int typeExtern){
    m_request.setUrl(url);
    // Send the request
    if(typeExtern == Network::None)
        goRequest(Network::Get);
    else
        goRequest(Network::Get, 0, typeExtern);
}

void Network::post(QUrl url, QByteArray data){
    m_request.setUrl(url);
    // Send the request
    goRequest(Network::Post, data);
}

void Network::postFile(QUrl url, QByteArray fileData){
    qDebug() << "Network::postFile  url = " << url << endl;
    m_request.setUrl(url);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data; boundary=" + QString(REQUEST_BOUNDARY)));
    // Send the request
    goRequest(Network::Post_File, fileData);
}

void Network::setCookies(QList<QNetworkCookie> cookies){
    QVariant var;
    var.setValue(cookies);
    // Set the cookies
    m_request.setHeader(QNetworkRequest::CookieHeader, var);
}
