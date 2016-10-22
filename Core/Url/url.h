#ifndef URL_H
#define URL_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QVariantMap>

#include <QDebug>

#include "../Secure/secure.h"


#define VPICTURE_BASE_API_URL "api.vpictu.re"
#define VPICTURE_WEBSITE_URL  "vpictu.re"

#define HTTPS   "https"
#define HTTP    "http"


class Url : public QObject
{
    Q_OBJECT
public:
    explicit Url(QObject *parent = 0);

signals:

public slots:
    /*
     * The Url::setKey function is called in the constructor of Network Class
     * It sets the Software API_KEY that is used when processing a request to the website: https://api.vpictu.re/
     */
    void setKey(QString API_KEY);

    void setUseHttps(bool enable=true);
    /*
     * This function initialize the m_url variable ==> argument actionType correspods to the ?action= type in php file
     * when urlBaseType is equal to 0, the link take the VPICTURE_BASE_API_URL as the base url, if it is different from 0, it takes the baseUrl argument as the base url
     */
    void url(QString actionType, QVariantMap arguments = QVariantMap(), QString baseUrl = QString());

    /*
     * Return the m_url value
     */
    QUrl getUrl();

    /*
     * Return the m_baseUrl
     */
    QUrl getBaseUrl(bool baseBaseUrl=true);

    /*
     * Add parameters to the url m_url
     */
    void addParameters(QString key, QString value);


private:
    QUrl        m_url;
    QUrlQuery   m_urlQuery;
    QString     m_baseUrl;
    bool        m_httpsEnabled;
    QString     m_PUBLIC_KEY;

    Secure      *secure;
};

#endif // URL_H
