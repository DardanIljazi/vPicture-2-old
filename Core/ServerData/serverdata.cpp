#include "serverdata.h"

ServerData::ServerData(QObject *parent) :
    QObject(parent)
{
}

void ServerData::setData(QVariantMap data){
    for(int i = 0; i < data.count(); i++){
        if(m_serverData.contains(data.keys().at(i)))
            m_serverData[m_serverData.keys().at(m_serverData.keys().indexOf(data.keys().at(i)))] = data.values().at(i);
        else
            m_serverData.insert(data.keys().at(i), data.values().at(i));
    }
}

QVariant ServerData::getValue(QString key){
    return m_serverData[key];
}

QMap<QString, QVariant> ServerData::returnPairs(QString key){
    return m_serverData[key].toMap();
}

QString ServerData::getUrl(){
    if(!m_serverData["url"].toString().isEmpty()){
        m_lastUrl = m_serverData["url"].toString();
        return m_lastUrl;
    }else // If there is a problem and the url key doesn't exist or is empty in the json code, we resend the last url m_lastUrl
        return m_lastUrl;
}

QVariantList ServerData::getNews(){
    if(!m_serverData["news"].toString().isEmpty()){
        m_lastNews = m_serverData["news"].toList();
        return m_lastNews;
    }else // If there is not last news
        return m_lastNews;
}

QVariantList ServerData::getUrlsForNews(){
    if(!m_serverData["urlsForNews"].toString().isEmpty()){
        m_lastUrlsForNews = m_serverData["urlsForNews"].toList();
        return m_lastUrlsForNews;
    }else // If there is not last urls for news
        return m_lastUrlsForNews;
}
