#include "settings.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    /*
     * The QSettings Class has not to be charged with parameters because:
     * QCoreApplication::setOrganizationName(), QCoreApplication::setOrganizationDomain() and QCoreApplication::setApplicationName()
     * have been set in main.cpp
     */
    m_settings = new QSettings;
    m_oldSettings = m_settings;
}

void Settings::addValue(QString key, QVariant value){
    m_settings->setValue(key, value);
}

void Settings::setValue(QString key, QVariant value){
    addValue(key, value);
}

QVariant Settings::getValue(QVariant key){
    if(m_settings->value(key.toString(), QVariant("NO_VALUE")) != QVariant("NO_VALUE"))
        return m_settings->value(key.toString());
    else
        return QVariant(-1); // Error value for key not found

}

QVariantMap Settings::getSettingsVariant(){
    for(int i = 0; i < m_settings->allKeys().count(); i ++)
        m_settingsVariant.insert(m_settings->allKeys().at(i), m_settings->value(m_settings->allKeys().at(i)));

    return m_settingsVariant;
}

//QVariant Settings::findKeyEnumValue(int i){
//    switch(i){
//    case Settings::USER_MAIL:
//        return getValue(R_USER_MAIL);
//        break;
//    case Settings::USER_PASSWORD:
//        return getValue(R_USER_PASSWORD);
//        break;
//    case Settings::USER_AUTOCONNECT:
//        return getValue(R_USER_AUTOCONNECT);
//        break;
//    case Settings::SOFTWARE_VERSION:
//        return getValue(R_SOFTWARE_VERSION);
//        break;
//    case Settings::SOFTWARE_LANGUAGE:
//        return getValue(R_SOFTWARE_LANGUAGE);
//        break;
//    case Settings::SOFTWARE_LINKFORMAT:
//        return getValue(R_SOFTWARE_LINKFORMAT);
//        break;
//    case Settings::SOFTWARE_DETECTPRINTSCREENKEY:
//        return getValue(R_SOFTWARE_DETECTPRINTSCREENKEY);
//        break;
//    case Settings::SOFTWARE_FIRSTUSE:
//        return getValue(R_SOFTWARE_FIRSTUSE);
//        break;
//    case Settings::PATHS_VPICTURE:
//        return getValue(R_PATHS_VPICTURE);
//        break;
//    case Settings::PATHS_FOLDERAPP:
//        return getValue(R_PATHS_FOLDERAPP);
//        break;
//    case Settings::PATHS_LASTDIR:
//        return getValue(R_PATHS_LASTDIR);
//        break;
//    case Settings::POSITIONS_REMEMBERPOS:
//        return getValue(R_POSITIONS_REMEMBERPOS);
//        break;
//    case Settings::POSITIONS_WPOSX:
//        return getValue(R_POSITIONS_WPOSX);
//        break;
//    case Settings::POSITIONS_WPOSY:
//        return getValue(R_POSITIONS_WPOSY);
//        break;
//    default:
//        return QVariant(-1);
//        break;
//    }
//    return QVariant(-1);
//}

//QString Settings::returnConcated(QString group, QString key){
//    // Check if the group or key has the '/' char.
//    if(group.contains("/") || key.contains("/"))
//        return group + key;
//    else
//        return group + "/" + key;
//}

void Settings::save(){
    m_settings->sync();
}

void Settings::setOldSettings(bool state){
    if(state == true){
        m_settings->clear();
        for(int i = 0; i < m_oldSettings->allKeys().count(); i++)
            m_settings->setValue(m_oldSettings->allKeys().at(i), m_oldSettings->value(m_oldSettings->allKeys().at(i)));

        save();
    }
}
