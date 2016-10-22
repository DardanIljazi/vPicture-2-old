#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent)
{
}

QList<QNetworkCookie> CookieJar::getAllCookies(){
    return allCookies();
}
