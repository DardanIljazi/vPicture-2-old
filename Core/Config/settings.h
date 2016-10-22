#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDebug>


//#define USER_GROUP      "user/"
//#define SOFTWARE_GROUP  "software/"
//#define PATHS_GROUP     "paths/"
//#define POSITIONS_GROUP "positions/"

//#define R_USER_MAIL                            returnConcated(USER_GROUP, "email")
//#define R_USER_PASSWORD                         returnConcated(USER_GROUP, "p")
//#define R_USER_AUTOCONNECT                      returnConcated(USER_GROUP, "autoConnect")
//#define R_SOFTWARE_VERSION                      returnConcated(SOFTWARE_GROUP, "version")
//#define R_SOFTWARE_LANGUAGE                     returnConcated(SOFTWARE_GROUP, "language")
//#define R_SOFTWARE_LINKFORMAT                   returnConcated(SOFTWARE_GROUP, "linkFormat")
//#define R_SOFTWARE_DETECTPRINTSCREENKEY         returnConcated(SOFTWARE_GROUP, "detectPrintScreenKey")
//#define R_SOFTWARE_FIRSTUSE                     returnConcated(SOFTWARE_GROUP, "firstUse")
//#define R_PATHS_VPICTURE                        returnConcated(PATHS_GROUP, "vPicture")
//#define R_PATHS_FOLDERAPP                       returnConcated(PATHS_GROUP, "folderApp")
//#define R_PATHS_LASTDIR                         returnConcated(PATHS_GROUP, "lastDir")
//#define R_POSITIONS_REMEMBERPOS                 returnConcated(PATHS_GROUP, "rememberPos")
//#define R_POSITIONS_WPOSX                       returnConcated(PATHS_GROUP, "wPosX")
//#define R_POSITIONS_WPOSY                       returnConcated(PATHS_GROUP, "wPosY")



/*
 * Settings class contains the settings for the software
 */

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
signals:

public slots:
    void addValue(QString key, QVariant value);
    void setValue(QString key, QVariant value);
    /*
     * Return the QVariant value for the value i found in enum Keys{}
     */
//    QVariant findKeyEnumValue(int i);
    void save();
    QVariant getValue(QVariant key);

    QVariantMap getSettingsVariant();
    void setOldSettings(bool state=true);


private slots:
//    QString returnConcated(QString group, QString key);

private:
    QSettings   *m_settings, *m_oldSettings;
    QVariantMap m_settingsVariant;

};

#endif // SETTINGS_H
