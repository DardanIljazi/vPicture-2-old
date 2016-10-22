#ifndef SERVERDATA_H
#define SERVERDATA_H

#include <QObject>
#include <QVariantMap>

#include <QDebug>

class ServerData : public QObject
{
    Q_OBJECT
public:
    explicit ServerData(QObject *parent = 0);

signals:

public slots:
    /*
     * Function called when server response to request arrives into the Class Parser, function Parser::parse
     */
    void setData(QVariantMap data);

    /*
     * Returns the value asked
     */
    QVariant getValue(QString key);

    /*
     * Return the value and the key
     */
    QMap<QString, QVariant> returnPairs(QString key);

    /*
     * Return the last url m_lastUrl
     */
    QString getUrl();

    /*
     * Return the last news
     */
    QVariantList getNews();

    /*
     * Return the last urls in reference to m_lastNews
     */
    QVariantList getUrlsForNews();

private:
    /*
     * m_serverData contains all the informations received by the server json response
     */
    QVariantMap     m_serverData;
    QString         m_lastUrl;
    QVariantList    m_lastNews;
    QVariantList    m_lastUrlsForNews;

};

#endif // SERVERDATA_H
