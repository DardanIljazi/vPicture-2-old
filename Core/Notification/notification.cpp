#include "notification.h"

Notification::Notification(QObject *parent, ServerData *serverData, Errors *errors) :
    QObject(parent)
{
    m_nbNotifications = 0;
    localServerData = serverData;
    localErrors     = errors;
}

QMultiMap<int, QPair<QString, QUrl> > Notification::getNotifications(){
    return m_notification;
}

void Notification::addNotification(int type, QString text, QUrl url){
    QPair<QString, QUrl> pair = qMakePair(text, url);

    if(!existYet(pair)){
        setNotSeenNotification();

        // Append this notification to the m_notifications var
        if(type == Notification::Notif_Notif){
            m_notification.insert(type, qMakePair(text, QUrl()));
        }
        // Get all news and insert them in m_notification
        else if(type == Notification::News_Notif){
            foreach (QVariant news, localServerData->getNews()) {
                static int i = 0;
                m_notification.insert(type, qMakePair(news.toString(), QUrl(localServerData->getUrlsForNews().at(i).toString())));
                i++;
            }
        }
        // Get the errors in the log file
        else if(type == Notification::Errors_Notif){
            m_notification.insert(type, qMakePair(localErrors->getLastError(), QUrl()));
        }
    }else{
        if(m_nbNotifications == 0){
            setNotSeenNotification();
        }
    }
}

void Notification::addNotifications(QList<QPair<int, QString> > listNotifications){
    for(int i = 0; i < listNotifications.count(); i++){
        addNotification(listNotifications.at(i).first, listNotifications.at(i).second);
    }
}

void Notification::setNotSeenNotification(int nbNotification, bool add){
    if(!add){
        m_nbNotifications = 0;
        emitNotSeenNotification(0);
    }
    else{
        m_nbNotifications += nbNotification;
        emitNotSeenNotification(m_nbNotifications);
    }
}

void Notification::emitNotSeenNotification(int i){
    emit notSeenNotifications(i);
}

void Notification::allNotificationsSeen(){
    setNotSeenNotification(0, true); // The m_nbNotification will be set to 0
}

bool Notification::existYet(QPair<QString, QUrl> pair){
    qDebug() << "pair: " << pair << "m_notification: " << m_notification.values() << endl;
    if(m_notification.values().contains(pair))
        return true;
    else
        return false;
}


