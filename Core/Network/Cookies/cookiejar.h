#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>


class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);

signals:

public slots:
    QList<QNetworkCookie> getAllCookies();

};

#endif // COOKIEJAR_H
