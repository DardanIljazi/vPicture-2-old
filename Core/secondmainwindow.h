#ifndef SECONDMAINWINDOW_H
#define SECONDMAINWINDOW_H

#include <QMainWindow>
#include <QMultiMap>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QListWidgetItem>
#include <QSignalMapper>
#include <QDesktopServices>
#include <QScrollBar>
#include <QMetaObject>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QTranslator>

#include "Notification/notification.h"
#include "Config/settings.h"
#include "Config/config.h"
#include "ServerData/serverdata.h"
#include "Network/network.h"
#include "Parser/parser.h"
#include "Thread/Thread.h"
#include "Thread/worker.h"
#include "Errors/errors.h"

namespace Ui {
class SecondMainWindow;
}

class SecondMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecondMainWindow(QWidget *parent = 0, Notification *notification=0, Settings *settings=0, ServerData *serverData=0, Network *network=0, Parser *parser=0, Config *config=0);
    ~SecondMainWindow();

    enum Stacked_Widget_Page{
        Notification_Page,
        Last_Pictures_Page,
        Settings_Page,
        Account_Page
    };

signals:
    void copyLink(QString link);
    void readyToThread(QStringList *listPathImages, QListWidget *listWidget, QSignalMapper *m_signalMapper, QList<QPushButton *> *linksButtons, QList<QPushButton *> *copyButtons);
    void languageChanged_(QString name, QTranslator *translator_);

protected:
    /*
     * Function called when a leaveEvent
     */
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    /*
     * Function called when the user exit the app
     */
    void closeEvent(QCloseEvent *event);

    void showEvent(QShowEvent *event);
public slots:
    void showStackedWidgetAt(int i);

/***** NOTIFICATION *****/
    /*
     * Function called when the Notification is shown
     */
    void initNotificationPage();
    /*
     * Function that returns the path to the icon
     */
    QString returnIconPath(int i);
    /*
     * Function called when the notiications are seen
     * This function will reinitialize the Notification class var Notification::m_nbNotifications to 0
     */
    void setAllSeenNotifications(bool state=true);
/********************/

/***** SETTINGS *****/
    /*
     * Function called when a item in ui->listWidget_Settings is pressed
     */
    void changeStackedWidgetSettings(QListWidgetItem* item);
    void changeStackedWidgetSettings(int i=0);
    /*
     * Function called to set all objects value with the settings class
     */
    void refreshUiSettings();
    /*
     * Function called to update settings with the interface values
     */
    void updateSettings();
    /*
     * Function called to save settings
     */
    void saveSettings();
    /*
     * Function called when the language is changed
     */
    void languageChanged(int newLanguage);

/********************/

/***** PICTURES *****/
    /*
     * Function that call the localNetwork->getImagesWithDate()); function
     */
    void localNetworkGetImagesWidhDate();
    /*
     * Initiates pictures into listWidget
     */
    void initiateLinksAndPictures(QStringList links);
    /*
     * Funcion called in initiateLinksAndPictures() to add QListWidgetItem with links in the listWidget_pictures
     */
    void addItems();
    /*
     * Function that returns the local path of the link (http://vpictu.re/4asd)
     * This will take the name behind the last / and will try all extensions that are allowed in the Config class Config::getListMimeDataAllowed()
     */
    QString getHttpLocalFilePath(QString httpLink);
    /*
     * Funciton called when a signal is mapped from m_signalMapper
     * This will detect which is the best way to procede with the action
     */
    void procedeSignalMapper(QString string);
    /*
     * Function called in procedeSignalMapper when it appears that a pushButton had been pressed
     */
    void openLink(QString link);
    /*
     * Function called each time scrollbar change its value
     * This function will see if the value is set to maximum, in which case it will reload others pictures
     */
    void changeItemSelected(int val);
    /*
     * Function that return the list of links that are yet in the listWidget_Pictures
     */
    QStringList getCurrentLinksInListWidget();
    /*
     * Function called after that a copy button has been pressed
     * In this case, the button will be set to tr("Copied!").
     * This function will se the tr("Copied!") to its normal state: tr("copy link")
     */
    void setPushButtonTextToNormalState();
    void addButtons(QString link, int i, QListWidgetItem *item);

    void doubleClick(QListWidgetItem *item);

/********************/


private:
    Ui::SecondMainWindow *ui;

    QPoint          m_lastPoint;
    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.
    QVariantMap     settingsBeginImage, settingsEndImage;

    // Pictures
    QStringList picturesLinks;
    QScrollBar *scrollBar;
    QList<QPushButton*> linksButtons;
    QList<QPushButton*> copyButtons;
    QList<int> indexOfLastCopyButtonPressed;

    QSignalMapper *m_signalMapper;

    QThread workerThread;

    QTranslator translator;


    Notification     *localNotification;
    Settings         *localSettings;
    Config           *localConfig;
    ServerData       *localServerData;
    Network          *localNetwork;
    Parser           *localParser;
    Worker           *localWorker;
    Thread           *localThread;
    Errors           *errors;
};

#endif // SECONDMAINWINDOW_H
