#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

#include "../Parser/parser.h"
#include "../Errors/errors.h"
#include "../Url/url.h"
#include "../Secure/secure.h"
#include "../ServerData/serverdata.h"
#include "Cookies/cookiejar.h"
#include "../Config/settings.h"


#define REQUEST_BOUNDARY "vPicture"


class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = 0, Errors *errors = 0, Parser *parser = 0, Url *url = 0, Settings *settings = 0, ServerData *serverData=0);

    enum Http_Request{
        None,
        Get,
        Post,
        Post_File,
        Download_Update
    };

signals:
    /*
     * Signal emitted in the Network::uploadProgression() function.
     * This will be used in the MainWindow class by the ui->progressBar_AfterUpload to change its progress
     */
    void upload(qint64 bytesReceived, qint64 bytesTotal);
    /*
     *
     * This will be used in the Updates class by the ui->progressBar_AfterUpload to change its progress
     */
    void upload_update(qint64 bytesReceived, qint64 bytesTotal);
    /*
     * This signal return all the data of the new setup software
     */
    void setupReceived(QByteArray setupData);

public slots:
    /*
     * Called when the reply of m_accessManager is finished
     */
    void replyFinished();
    /*
     * Called when the download of the new setup is done
     */
    void downloadFinished();
    /*
     * If we want to modify some of request headers
     */
    void setRawHeaders( const QByteArray & headerName, const QByteArray & headerValue );
    void setUseSsl(bool enable=true);
    /*
     * If errors occur when reply is sent
     */
    void replyErrors(QNetworkReply *response);


    /*
     * Used as a main function when GET, POST and others request are sent
     */
    void goRequest(int type, QByteArray data=0, int typeExtern=Network::None);


    /******** REQUEST ********/
    /*
     * For GET Http request. Argument: url
     */
    void get(QUrl url, int typeExtern=Network::None);
    /*
     * For POST Http request. Argument: url, data ==> the data to be send
     */
    void post(QUrl url, QByteArray data);
    /*
     * For POST Http request only for files. Argument: url, data ==> the data of the file in byte
     */
    void postFile(QUrl url, QByteArray fileData);
    /*
     * Function called in Network::replyFinished() function.
     * It set the cookies received by the server to the QNetworkRequest variable  m_request
     */
    void setCookies(QList<QNetworkCookie> cookies);
    /*
     * Function called when the upload begins.
     * This function is needed to set the progress of the ui->progressBar_AfterUpload in the MainWindow class
     */
    void uploadProgression(qint64 bytesReceived, qint64 bytesTotal);
    /************************/


    /******** CONNECTION ********/
    /*
     * Connection to the server for the user and receive the account_id and others variables sent in the connection
     * The second argument can be crypted or not. Function check if the second argument is already crypted with SHA1 method (See Secure::secure function) or not.
     */
    void connectUser(QString mail, QString password);
    /*
     * Check if the second paramter for Network::connectUser() function (QString password) is crypted or not and return boolean value
     */
    bool isPasswordCrypted(QString password);
    /************************/

    /******** LINKS ********/
    void getImagesWithDate(int minday=0);
    /***********************/


private:
    QNetworkAccessManager   *m_accessManager;
    QNetworkRequest          m_request;
    QNetworkReply           *m_reply;

    Errors      *localErrors;
    Parser      *localParser;
    Url         *localUrl;
    Secure      *secure;
    CookieJar   *cookieJar;
    Settings    *localSettings;
    ServerData  *localServerData;
};

#endif // NETWORK_H
