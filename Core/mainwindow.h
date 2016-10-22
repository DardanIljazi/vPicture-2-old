#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QLocale>
#include <QDir>
#include <QMovie>
#include <QDesktopServices>
#include <QFileDialog>
#include <QClipboard>
#include <QSignalMapper>
#include <QMenu>
#include <QPropertyAnimation>
#include <QMdiArea>
#include <QtNetwork>
#include <QListWidgetItem>

#include "SingleApplication/singleapplication.h"
#include "Network/network.h"
#include "Errors/errors.h"
#include "Config/settings.h"
#include "Parser/parser.h"
#include "Url/url.h"
#include "Files/files.h"
#include "Config/config.h"
#include "Upload/upload.h"
#include "ServerData/serverdata.h"
#include "Modules/Screenshot/screenshot.h"
#include "Events/KeyEvent/systemkeyboardreadwrite.h"
#include "Parser/argumentsparser.h"
#include "../Design/Design/menu.h"
#include "Notification/notification.h"
#include "secondmainwindow.h"
#include "Modules/GifCreator/gifcreator.h"
#include "minioutwidget.h"
#include "Menu/menumain.h"
#include "Modules/OnlineAssistant/onlineassistant.h"
#include "Dialog/dialog.h"
#include "Updates/updates.h"

#ifdef Q_OS_WIN
#include <QtWinExtras>
#endif


#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum DropFiles_State{
        Valid_Files,
        Invalid_Files,
        None_File
    };

    enum Clipoard_Type{
        Image,
        Text,
        Pixmap
    };

    enum Window_Opacity{
        Show_Progressively,
        Hide_Progressively
    };

public slots:
    /***** FIRST USE *****/
        /*
         * Check if it is the first time the user run the software.
         */
        void checkFirstUse();
        /*
         * Function called in MainWindow::checkFirstUse function to initializes values for group/keys
         * In others case, it can be called when the user want to initialize his settings to default values
         */
        void initializeDefaultValues();
    /********************/

    /***** DESIGN *****/
        /*
        * Function that is called when the software runs and initializes design elements
        */
        void initDesignElements();
        /*
        * Function called when the user is connected and the QStackedWidget change his index
        */
        void changeStackedWidgetIndex(int i = 1);

        /*
        * Functions that initializes and end the animation when the user want to connect (the circle)
        */
        void initAnimationConnection();
        void endAnimationConnection();

        /*
        * Function called before the user is connected (to do the transition between the first ui.centralStackedWidget and the second page)
        */
        void makeTransparentProgresively();
        /*
        * Function called when the second page for centralStackedWidget is set and that the window must be centred
        */
        void centerWindowToLastRect();
        /*
         * Function called when files are dropped on the window.
         * This function will change the middle picture by an other: if the files are accepted, it will by replaced by draganddrop_valid, else by draganddrop_invalid
         */
        void changeMiddlePictureState(int state);
        /*
         * Function called when the signal upload() from Network Class is called
         * This function will change the progression of the ui->progressBar_AfterUpload
         */
        void changeProgressBarProgression(qint64 bytesReceived, qint64 bytesTotal);
        /*
         * When we receive the url from the server, we show it the lineEdit_url of the widget widget_AfterUpload
         */
        void setLineEditUrl(QVariantMap img);
        /*
         * Function called when the ui->lineEdit_Url is selected (or its cursor positon changed)
         * This will select all the text in the lineEdit
         */
        void selectLineEditUrl(int old_pos=0, int new_pos=0);
        /*
         * Function called in the setLineEditUrl() function
         * This function will wait 150ms before the ui->widget_AfterUpload is shown
         */
        void waitBeforeWidgetAfterUploadShow();
        /*
         * Function called when the user click on the ui->toolButton_Copy to copy the url in the lineEdit_Url
         */
        void copyLineEditUrl();
        /*
         * Function called when the user want to copy something in the clipboard and that an animation must be shown
         */
        void copyLinkInClipBoardWithAnimation(QString link);
        /*
         * Function callaed after the url of ui->lineEdit_Url is copied
         */
        void animateLabelCopied();
        /*
         * Function called when the ui->toolButton_CloseAfterUpload is called (it closes/hide the ui->afterUpload widger)
         */
        void hideWidgetAfterUpload();
        /*
         * Function that shows the menu for the buttons like (Screenshot), Parameters or Menu
         */
        void showMenuFor(QWidget *widget);
        /*
         * Function called in the beginning of the application
         * It shows the window progressively or hide it progressively too
         */
        void showOrHideProgressively(int state=MainWindow::Show_Progressively);
        /*
         * This function is called in the showOrHideProgressively() function or else when we must know if the opacity is as we thought to be
         * In some OS, the opacity of widgets it is not supported (like Window CE) and on others OS specificity.
         * If the opacity appears not to be full (1.0 or opaque), we set it as opaque
         */
        void setOpacityIfNotSupported();
        /*
         * Functions that animate the list of social buttons when the mouse go on ui->shareSocialButton and show ui->widgetSocialButtons
         */
        void showWidgetSocialButtons();
        void hideWidgetSocialButtons();
    /********************/

    /***** PARSER *****/
         /*
         * Function called when the emit clientConnected is called in Parser::parse() function
         * This function means that the user has been accepted by the server
         */
         void clientConnected();
    /********************/

    /***** CLIPBOARD *****/
         /*
          * This function will copy some data in the clipboard
          */
         bool copyToClipboard(QVariant data, int type=MainWindow::Text);
    /********************/

    /***** SOCIAL *****/
        /*
         * Those functions share link or page by the social medias
         */
         void shareOnFacebook();
         void shareOnTwitter();
         void shareOnGooglePlus();
    /********************/

    /***** KEY_EVENT *****/
        /*
         * Function called when the user press a key under Q_OS_WIN
         */
        #ifdef Q_OS_WIN
            void analyzeKeySent(DWORD keyCode=0);
        #endif
    /*********************/

    /***** EVENTS *****/
        /*
         * Function called to know if the software must be hide and the miniOutWidget must be shown
         */
         bool isOutOfScreen();
         /*
          * Function called to corner the software on the right corner
          */
         void setOnRightCorner();
         /*
          * Function called to corner the software on the left corner
          */
         void setOnLeftCorner();
         /*
          * Function called when the signal miniOutWidget->logoPressedEvent() is emitted.
          * This function will show the application from the corner
          */
         void showFromCorner();
     /*********************/

    /***** UPLOAD *****/
        /*
         * Function called in the MainWindow::dragEnterEvent() function (DragAndDrop folder)
         * It initializes the list of urls and verifies if the extension is allowed
         */
        bool initializeUrls(QList<QUrl> urls);
        /*
         * Function used after initializesUrls.
         * When Drag (MainWindow::dragEnterEvent) and Drop (MainWindow::dropEvent) are called, the uploadFiles is called into dropEvent function
         * In others situation, this function is called after MainWindow::initializeUrls function
         */
        void uploadFiles();
        /*
         * Function called after clicking on ui->toolButton_DragAndDrop or on the "click" in ui->label_text
         * This function will call a QFileDialog where user can select the images to upload
         */
        void selectFileToUpload();
    /******************/

    /***** MENU PERSONALIZED *****/
        /*
         * Function that returns the actions for each menu
         */
        QList<QAction *> getActionsFor(QString widgetName);
        /*
         * Function called when the signal outOfFocus from m_customMenu is emitted
         * This function will set the m_isMenuActivated to false
         */
        void setMenuDesactivated();
        QString getFlatMenuDesign();
        void showProgressivelyMenu();
        void itemPressed(QListWidgetItem *item);
        void showMenuForLink();
    /*****************/

    /***** INTERNATIONALIZATION *****/
        void changeLanguage(QString name, QTranslator *translator = new QTranslator());
    /********************************/

    /***** UPDATES *****/
        void checkForUpdates(QString version="none");
        void setProgressionUpdates(qint64 bytesReceived, qint64 bytesTotal);
    /*******************/

    /***** SLOTS *****/
        /**
          Refered to Settings Class*/
        /*
         * Function called to save the last window position
         */
        void saveWindowPosition();
        /**
        ******************************/

        /**
          Refered to Screenshot Class*/
        /*
         * Function called when user want to take screenshot
         */
        void Screenshotscreenshot();
        /*
         * Function called when the screenshot->screenshotTaken() signal is emmited
         */
        void uploadScreenshot();
        /*
         * Function called when the signal screenshot->copyEvent() signal is emmited
         */
        void copyScreenshotPixmap();
        /**
        ******************************/

        /**
          Refered to GifCreator Class*/
        /*
         * Function called to initialize the GifCreator class
         */
        void initGifCreator();
        /**
        ******************************/

        /**
          Referer to Network Class*/
        /*
         * Send connection informations to Network::connectUser() function by passing arguments ui->lineEditEmail.text() and ui->lineEditPassword.text()
         * to network->connectUser();
         * Information: this slot is called when ui->buttonConnect is pressed.
         */
        void Networkconnectuser();
        /**
        ******************************/

        /**
          Refer to Windows actions*/
        /*
         * Function called when the ui->pushbutton_Reduce is called
         * This will minimize the windows
         */
        void minimizeWindow();

        /*
         * Function called after the windowState (minized) must be changed to normal mode
         * This function is called after the user want to show the software, after that he has been reduced
         */
        void showNormalWindow();
        /**
        ******************************/

        /**
          Refer to SecondMainWindow*/
            /*
             * Function called when the showNotificationPage function is called.
             * This function show progressively the windows
             */
            void showProgressivelySecondMainWindow();
       /**
       ******************************/

        /**
          Refer to Notification class*/
        /*
         * Function called when we want to show the Notification interface
         */
         void showNotificationPage();

        /*
         * Function called when the emit signal from Notificatio class Notification::notSeenNotifications() is emmited
         */
            void setNotYetSeenNotification(int i);
        /*
         * Function that animates the ui->toolButton_Logo_Notification and changes the color of it to dark
         */
            void animateLogoNotification();
         /*
          * When the user use the App for the first time, we want to share it welcome message
          */
            void notificationWelcomeMessage();
        /**
        ******************************/

        /**
          Refer to OnlineAssistant Class*/
            /*
             * This function show progressively the windows
             */
            void showProgressivelyOnlineAssitant();
        /**
        ******************************/

        /**
           Refer to Pictures Class*/
        /*
         * Function called when we want to show LastPictures interface
         */
            void showLastPicturePage();
        /**
        ******************************/

    /*****************/

signals:
    void transparentWindow();
    void opaqueWindow();

protected:
    /*
     * Functions called when user Drag and Drop a file
     */
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *);


    /*
     * Functions called when an event appears (MouseEvent, EnterEvent..)
     */
    bool event(QEvent *event);
    /*
     * Function called when a leaveEvent
     */
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    /*
     * This function will principaly be used for the event QEvent::LanguageChange.
     * LanguageChange is called when QCoreApplication::installTranslator() is set
     */
    void changeEvent(QEvent *event);

    /*
     * Function called when the user exit the app
     */
    void closeEvent(QCloseEvent *event);

    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;

    Errors           *errors;
    Settings         *settings;
    Parser           *parser;
    Network          *network;
    Url              *url;
    Files            *files;
    Config           *config;
    Upload           *upload;
    ServerData       *serverData;
    ScreenShot       *screenshot;
    Notification     *notification;
    SecondMainWindow *secondMainWindow;
    GifCreator       *gifCreator;
    MiniOutWidget    *miniOutWidget;
    OnlineAssistant  *onlineAssistant;
    Updates          *updates;

    QPoint          m_lastPoint;            // When leaving the software (in MainWindow::mouseMoveEvent and MainWindow::mousePressEvent)

    QMovie          *m_movie;               // Animation when connecting

    /***** MENU PERSONALIZED *****/
        QSignalMapper    *m_signalMapper;   // SignalMapper that is connected with toolbuttons on the top of the app (Screenshot, Parameters, Menu..)
        MenuMain         *m_Menu;           // Custom Menu for (Screenshot), Parameters, Menu...
        bool m_isMenuActivated;             // m_isMenuActived is a variable that is true when the menu is shown and false when not
    /************************/

   /***** NOTIFICATION *****/
        QPropertyAnimation *propertyAnimation_Logo_Notification;
   /************************/

   /***** EVENTS *****/
        bool cornered;
   /******************/

   /*** INTERNATIONALIZATION ***/
        QTranslator *localTranslator;
   /****************************/

   /*** UPDATES ***/
#ifdef Q_OS_WIN
    QWinTaskbarButton   *winTaskBarButton;
    QWinTaskbarProgress *winTaskBarProgress;
#endif
   /***************/

    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.

    /***** KEY EVENT *****/
#ifdef Q_OS_WIN
    SystemKeyboardReadWrite *keyEvent;
#endif
    /************************/

    QClipboard *clipboard;

};

#endif // MAINWINDOW_H
