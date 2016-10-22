#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>
#include <QMultiMap>
#include <QPair>
#include <QUrl>

#include "../ServerData/serverdata.h"
#include "../Errors/errors.h"

class Notification : public QObject
{
    Q_OBJECT

public:
    explicit Notification(QObject *parent = 0, ServerData *serverData=0, Errors *errors=0);

    enum Notification_Category{
        News_Notif,           // News,    uses data received by the server
        Errors_Notif,         // Errors,  uses error log data
        Notif_Notif,          // Default, uses QString value
        Warning_Notif
    };

signals:
    void notSeenNotifications(int);

public slots:
    /*
     * Return the m_notification list of notifications
     */
    QMultiMap<int, QPair<QString, QUrl> > getNotifications();

    /*
     * Add a notification with type, the default value of type is Notification by default
     */
    void addNotification(int type=Notification::Notif_Notif, QString text="", QUrl url = QUrl());
    /*
     * Add notifications
     */
    void addNotifications(QList< QPair<int, QString> > listNotifications);

    /*
     * Add the number of notification not seen
     */
    void setNotSeenNotification(int nbNotification = 1, bool add=true);

    /*
     * Function that is called when the notifications are seen
     */
    void allNotificationsSeen();

    /*
     * Function that calls the emit
     */
    void emitNotSeenNotification(int i);

    /*
     * Function that controlls if the new notification exists yet
     * If it exists, no action will be done (no add of new notification)
     */
    bool existYet(QPair<QString, QUrl> pair);

private:
    /*
     * m_notification is a list of all notifications that have been done during the process of the software
     * The int argument is the type of notification (See the enum Notification_Category)
     * The second argument (QMap<QString, QUrl>), contains an url and a string. The QString parameter is the text that is shown and the QUrl,
     */
    QMultiMap<int, QPair<QString, QUrl> > m_notification;

    /*
     * m_nbNotifications is the number of notifications that have not been seen by the user
     * The maximum value must be 9, else it will be +9
     */
    int m_nbNotifications;

    ServerData      *localServerData;
    Errors          *localErrors;
};

#endif // NOTIFICATION_H
