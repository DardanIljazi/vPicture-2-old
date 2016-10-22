#include "url.h"

Url::Url(QObject *parent) :
    QObject(parent)
{
    m_httpsEnabled = false;

    secure       = new Secure;
}

void Url::setKey(QString API_KEY){
    m_PUBLIC_KEY = API_KEY;
}

void Url::url(QString actionType, QVariantMap arguments, QString baseUrl){

    m_url.clear();
    m_urlQuery.clear();

    if(m_httpsEnabled)
        m_url.setScheme(HTTPS);
    else
        m_url.setScheme(HTTP);

    // The urlBaseType is keeped as default (VPICTURE_BASE_API_URL)
    if(baseUrl.isEmpty())
        m_url.setHost(VPICTURE_BASE_API_URL); // ==>       api.vpictu.re
    else
        m_url = QUrl(baseUrl);               // ==> f.ex: www.vpictu.re


    if(!actionType.isEmpty()){
        addParameters("action", actionType); // f.example: action=sendimages
        addParameters("key", m_PUBLIC_KEY);
    }

    if(arguments.count() > 0){
        for(int i = 0; i < arguments.count(); i++)
            addParameters(arguments.keys().value(i), arguments.values().at(i).toString());
    }
    /* Called to set the m_baseUrl */
    getBaseUrl();
    qDebug() << "Url::url  m_url: " << m_url << endl;
}

void Url::addParameters(QString key, QString value){
    m_urlQuery.addQueryItem(key, value);
    m_url.setQuery(m_urlQuery);
}

QUrl Url::getBaseUrl(bool baseBaseUrl){
    if(baseBaseUrl)
        m_baseUrl = m_url.host(QUrl::RemoveScheme | QUrl::StripTrailingSlash | QUrl::RemovePort);
    else
        m_baseUrl = m_url.host(QUrl::RemoveScheme | QUrl::StripTrailingSlash);

    qDebug() << "m_baseUrl: " << m_baseUrl << endl;
    return m_baseUrl;
}

QUrl Url::getUrl(){
    return m_url.url();
}

void Url::setUseHttps(bool enable){
    m_httpsEnabled = enable;
}
