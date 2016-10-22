#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * Todo list
 * TODO: GifCreator
 * TODO: Picture Edit Module
 * TODO: Update module
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::WindowFlags(Qt::Window | Qt::FramelessWindowHint)),
    ui(new Ui::MainWindow)
{
    QFile::remove("errors.log");
    errors          = new Errors;


    ui->setupUi(this);

    clipboard = QApplication::clipboard();


    /*
     * Function that initializes some basic design, for example antialiasing on some ui labels
     */
    initDesignElements();


    ui->centralStackedWidget->setCurrentIndex(1);

    //    changeStackedWidgetIndex(1);
    setAttribute(Qt::WA_TranslucentBackground);


    QList<QUrl> listFiles;
    /*
     **** %{listFiles} *****
     * Get all the files passed in arguments
     ***********************
     */
    for(int i = 0; i < QApplication::arguments().count(); i ++){
        if(QFile::exists(QApplication::arguments().at(i))){
            /* Under Windows, it is possible that the first argument (arguments.at(0)) is equal to the software application path
             * In this case, we verify if the arguments is equal to this value (QApplication::applicationFilePath()) */
            if(QFileInfo(QApplication::arguments().at(i)).canonicalFilePath() != QFileInfo(QApplication::applicationFilePath()).canonicalFilePath())
                listFiles << QUrl::fromUserInput(QApplication::arguments().at(i));
        }
    }
    /**********************/


    //    SingleApplication app;
    //    if(!app.shouldContinue())qApp->exit();

    // We allow the Drop events
    this->setAcceptDrops(true);
    this->setMouseTracking(true);


    serverData      = new ServerData;

    settings        = new Settings;

    // We look if it is the first time the user run the application or if the versions doesn't correspond
    checkFirstUse();

    parser          = new Parser(0, serverData, settings);

    config          = new Config;

    files           = new Files(0, errors);

    url             = new Url;

    url->setUseHttps(true);
    //    url->setUseHttps(false);

    url->setKey("d6bd9042d524394dd53ee0144d37d967134bb0887ccb3832d77752cf7accf00e"); // API_KEY == PUBLIC_KEY

    network         = new Network(0, errors, parser, url, settings, serverData);

    network->setUseSsl(true);
//    network->setUseSsl(false);

    upload          = new Upload(0, errors, files, network, url);

    screenshot      = new ScreenShot;

    notification    = new Notification(0, serverData, errors);

    secondMainWindow= new SecondMainWindow(0, notification, settings, serverData, network, parser, config);

    //    QTimer::singleShot(2000, secondMainWindow, SLOT(initiateLinksAndPictures()));
    miniOutWidget   = new MiniOutWidget;

    miniOutWidget->hide();

    m_Menu          = new MenuMain();

    m_Menu->hide();
    onlineAssistant = new OnlineAssistant();

    onlineAssistant->hide();
    updates         = new Updates();

    updates->hide();

#ifdef Q_OS_WIN
    QWinThumbnailToolBar *thumbbar = new QWinThumbnailToolBar(this);
    thumbbar->setWindow(this->windowHandle());

    QWinThumbnailToolButton *screenshotThumbnail = new QWinThumbnailToolButton(thumbbar);
    screenshotThumbnail->setToolTip("Screenshot");
    screenshotThumbnail->setIcon(QIcon(":/Screenshot/screenshot"));
    screenshotThumbnail->setDismissOnClick(true);
    connect(screenshotThumbnail, SIGNAL(clicked()), this, SLOT(Screenshotscreenshot()));

    QWinThumbnailToolButton *picturesThumbnail = new QWinThumbnailToolButton(thumbbar);
    picturesThumbnail->setToolTip(tr("Pictures"));
    picturesThumbnail->setIcon(QIcon(":/Urls/urls_icon"));
    connect(picturesThumbnail, SIGNAL(clicked()), this, SLOT(showLastPicturePage()));

    thumbbar->addButton(screenshotThumbnail);
    thumbbar->addButton(picturesThumbnail);
#endif


    QTimer::singleShot(4000, this, SLOT(checkForUpdates()));




    pressed = false;

    if(settings->getValue("positions/rememberPos").toBool()){
        this->move(settings->getValue("positions/wPosX").toInt(), settings->getValue("positions/wPosY").toInt());
        // Check if the windows was out of screen to show the minioutwidget in this case
        if(isOutOfScreen()){
            this->cornered = true;
            miniOutWidget->show();
        }
    }




#ifdef Q_OS_WIN
    if(!QApplication::arguments().contains("noenumw")){
        keyEvent    = SystemKeyboardReadWrite::instance();
        keyEvent->setConnected(true);
        QObject::connect(keyEvent, SIGNAL(keyPressed(DWORD)), this, SLOT(analyzeKeySent(DWORD)));
    }
#endif


    /****** PARSER *****/
    /*
     * When the signal that the user is connected is received by the signal
     */
    QObject::connect(parser, SIGNAL(clientConnected()), this, SLOT(clientConnected()));
    /*
     * When the url is sent by the server, the signal Parser::urlReceived() is emitted
     */
    QObject::connect(parser, SIGNAL(urlReceived(QVariantMap)), this, SLOT(setLineEditUrl(QVariantMap)));
    /*
     * When the version is returned by the server
     */
    QObject::connect(parser, SIGNAL(versionReceived(QString)), this, SLOT(checkForUpdates(QString)));
    // When user triggers the actionScreenshot action in tabBar
    QObject::connect(ui->actionScreenshot, SIGNAL(triggered()), ui->toolButton_Screenshot, SIGNAL(clicked()));
    QObject::connect(ui->toolButton_Screenshot, SIGNAL(clicked()), this, SLOT(Screenshotscreenshot()));
    // When client press the connect button on the interface
    //    QObject::connect(ui->buttonConnect, SIGNAL(pressed()), this, SLOT(Networkconnectuser()));
    // When the index of the QStackedWidget changes
    QObject::connect(ui->centralStackedWidget, SIGNAL(currentChanged(int)), this, SLOT(changeStackedWidgetIndex(int)));
    // When the "click" in the label_text is clicked or on the toolButton_DragAnDrop
    QObject::connect(ui->label_text, SIGNAL(linkActivated(QString)), this, SLOT(selectFileToUpload()));
    QObject::connect(ui->toolButton_DragAndDrop, SIGNAL(pressed()), this, SLOT(selectFileToUpload()));
    // When the transparentWidget() signal is emmited that means that the stackedWidget must change
    QObject::connect(this, SIGNAL(transparentWindow()), this, SLOT(changeStackedWidgetIndex()));
    // When the cursor of the ui->lineEdit_Url is changed (or simply clicked), all the text of this lineEdit is selected
    QObject::connect(ui->lineEdit_Url, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(selectLineEditUrl(int,int)));
    QObject::connect(ui->toolButton_CopyUrl, SIGNAL(pressed()), this, SLOT(copyLineEditUrl()));
    /*** Top Buttons Exit and Reduce for the two OS (Windows, Unix) ***/
    QObject::connect(ui->pushButton_Exit_2, SIGNAL(pressed()), ui->pushButton_Exit, SIGNAL(pressed()));
    QObject::connect(ui->pushButton_Reduce_2, SIGNAL(pressed()), ui->pushButton_Reduce, SIGNAL(pressed()));
    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(close()));
    QObject::connect(ui->pushButton_Reduce, SIGNAL(pressed()), this, SLOT(minimizeWindow()));

    QObject::connect(ui->toolButton_Pictures, SIGNAL(pressed()), this, SLOT(showLastPicturePage()));
    /*** Signal Mapper for (Screenshot), Parameters, Menu toolButtons ***/
    QObject::connect(ui->toolButton_Menu, SIGNAL(pressed()), this, SLOT(showProgressivelyMenu()));
    QObject::connect(ui->toolButton_ArrowAfterUpload, SIGNAL(pressed()), this, SLOT(showMenuForLink()));
    QObject::connect(secondMainWindow, SIGNAL(copyLink(QString)), this, SLOT(copyLinkInClipBoardWithAnimation(QString)));
    //    m_signalMapper = new QSignalMapper(this);
    ////    m_signalMapper->setMapping(ui->toolButton_Parameters, ui->toolButton_Parameters);
    //    m_signalMapper->setMapping(ui->toolButton_Menu, ui->toolButton_Menu);
    //    QObject::connect(ui->toolButton_Parameters, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    //    QObject::connect(ui->toolButton_Menu, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    //    QObject::connect(m_signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(showMenuFor(QWidget*)));
    QObject::connect(m_Menu->listWidget(), SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(itemPressed(QListWidgetItem*)));
    QObject::connect(secondMainWindow, SIGNAL(languageChanged_(QString,QTranslator*)), this, SLOT(changeLanguage(QString,QTranslator*)));
    /**
    ******************************/

    /**
     * Referenced to Social buttons*/
    QObject::connect(ui->toolButton_ShareSocial, SIGNAL(pressed()), this, SLOT(showWidgetSocialButtons()));
    //    QObject::connect(ui->facebook1, SIGNAL(pressed()), ui->facebook2, SIGNAL(pressed()));
    //    QObject::connect(ui->twitter1, SIGNAL(pressed()), ui->twitter2, SIGNAL(pressed()));
    //    QObject::connect(ui->googleplus1, SIGNAL(pressed()), ui->googleplus2, SIGNAL(pressed()));

    QObject::connect(ui->facebook2, SIGNAL(pressed()), this, SLOT(shareOnFacebook()));
    QObject::connect(ui->twitter2, SIGNAL(pressed()), this, SLOT(shareOnTwitter()));
    QObject::connect(ui->googleplus2, SIGNAL(pressed()), this, SLOT(shareOnGooglePlus()));
    /**
    ******************************/

    /**
     * Referenced to Network signals*/
    // Set the progression of the ui->progressBar_AfterUpload
    QObject::connect(network, SIGNAL(upload(qint64,qint64)), this, SLOT(changeProgressBarProgression(qint64,qint64)));
    // Set the progression of the ui->progressBar_AfterUpload in Updates
    QObject::connect(network, SIGNAL(upload_update(qint64,qint64)), this, SLOT(setProgressionUpdates(qint64,qint64)));
    QObject::connect(network, SIGNAL(setupReceived(QByteArray)), updates, SLOT(saveAndLaunchSetup(QByteArray)));
    /**
    ******************************/

    /**
      * Referenced to Nottication signals*/
    // Show the ui.toolButton_Notification with int value
    QObject::connect(notification, SIGNAL(notSeenNotifications(int)), this, SLOT(setNotYetSeenNotification(int)));
    QObject::connect(ui->toolButton_Notification, SIGNAL(pressed()), ui->toolButton_Logo_Notification, SIGNAL(pressed()));
    QObject::connect(ui->toolButton_Logo_Notification, SIGNAL(pressed()), this, SLOT(showNotificationPage()));
    /**
    ******************************/

    /**
      * Referenced to Screenshot signals*/
    QObject::connect(screenshot, SIGNAL(screenshotTaken()), this, SLOT(uploadScreenshot()));
    QObject::connect(screenshot, SIGNAL(copyEvent()), this, SLOT(copyScreenshotPixmap()));
    QObject::connect(screenshot, SIGNAL(gifCreatorEvent()), this, SLOT(initGifCreator()));
    /**
    ******************************/

    /**
      * Referenced to MiniOutWidget signals*/
    QObject::connect(miniOutWidget, SIGNAL(logoPressedEvent()), this, SLOT(showFromCorner()));
    QObject::connect(miniOutWidget, SIGNAL(screenshotEvent()), this, SLOT(Screenshotscreenshot()));
    QObject::connect(miniOutWidget, SIGNAL(picturesEvent()), this, SLOT(showLastPicturePage()));
    /**
    ******************************/
    /****************************/


}


#ifdef Q_OS_WIN
void MainWindow::analyzeKeySent(DWORD keyCode){
    // If the print screen key has been pressed
    if(QString::number(keyCode) == QString::number(VK_SNAPSHOT)){
        Screenshotscreenshot();
    }
}
#endif

/***** SCREENSHOT *****/
void MainWindow::Screenshotscreenshot(){

    screenshot->refreshWindowsPos();

    screenshot->screenshot();

}

void MainWindow::uploadScreenshot(){
    // Show progress bar
    changeProgressBarProgression(0, 0);


    upload->uploadPixmap(screenshot->getLastScreenshot());

}

void MainWindow::copyScreenshotPixmap(){

    copyToClipboard(screenshot->getLastScreenshot(), MainWindow::Pixmap);

    QTimer::singleShot(650, this, SLOT(animateLabelCopied()));

}

/**********************/

/***** GIFCREATOR *****/
void MainWindow::initGifCreator(){

    gifCreator = new GifCreator(0, settings);

    gifCreator->setRect(screenshot->lastSelectRect());

    gifCreator->show();

}

/**********************/


/***** NETWORK *****/
void MainWindow::Networkconnectuser(){
    //    network->connectUser(ui->lineEditMail->text(), ui->lineEditPassword->text());

    // Hide the connectsWidget before using
    //    ui->connectsWidget->hide();

    // Initializes the animation for connectig
    initAnimationConnection();
}
/*******************/

/****** FIRST USE *****/
void MainWindow::checkFirstUse(){

    if(settings->getValue("software/firstUse") == -1 || settings->getValue("software/firstUse").toBool() == true || settings->getValue("software/version").toString() != QCoreApplication::applicationVersion()){

        // If it is the first time the user run the software, we initialize all default values for group/keys
        settings->setValue("software/firstUse", false);

        initializeDefaultValues();


        // Here, it is something special, we want to share to the user a welcome message through the notification system
        // For technicals details (due to Signal and Slots system that are not still declared), we have to wait some time to send it to the user. In case to have something well done, we prefer to send it after 8 seconds
        QTimer::singleShot(7500, this, SLOT(notificationWelcomeMessage()));
    }
}

void MainWindow::initializeDefaultValues(){

    settings->setValue("user/autoConnect", true);

    settings->setValue("software/version", QCoreApplication::applicationVersion());

    //    settings->setValue("software/language", localeLanguage.languageToString(localeLanguage.language()));
    settings->setValue("software/language", QLocale::system().name().section("_", 0, 0));

    this->changeLanguage(QLocale::system().name().section("_", 0, 0));

    settings->setValue("software/linkFormat", "%{link}");

    settings->setValue("software/detectPrintScreenKey", true);

    settings->setValue("paths/vPicture", QApplication::applicationFilePath());

    settings->setValue("paths/folderApp", QApplication::applicationDirPath());


#ifdef Q_OS_WIN
    settings->setValue("paths/lastDir", QFileInfo(QString(QDir::homePath() + "/Pictures/")).canonicalFilePath());
#else
    ssettings->setValue("paths/lastDir", QFileInfo(QDir::homePath()).canonicalPath());
#endif


    settings->setValue("positions/rememberPos", false);

    settings->setValue("positions/wPosX", false);

    settings->setValue("positions/wPosY", false);

    settings->setValue("screenshot/useWid", true);

    settings->setValue("gifcreator/filePerSecond", 10);

    settings->setValue("gifcreator/showCursor", false);


    settings->setValue("design/type", "white");

    settings->save();

}
/****************************/

/***** PARSER *****/
void MainWindow::clientConnected(){
    /* Client is now connected to the server: and we must:
     * Stop the m_movie (the animation when connection is established)
     * Change the ui->centralStackedWidget
     */
    endAnimationConnection();
}
/*******************/

/***** CLIPBOARD *****/
bool MainWindow::copyToClipboard(QVariant data, int type){

    if (type == MainWindow::Image || type == MainWindow::Pixmap) {

        clipboard->setPixmap(data.value<QPixmap>());

    }else if (type == MainWindow::Text) {

        clipboard->setText(data.toString());

    }else {

        clipboard->setText(tr("Cannot display data"));

    }



    return true;
}
/*******************/

/***** SOCIAL *****/
void MainWindow::shareOnFacebook(){

    QDesktopServices::openUrl(QUrl("http://www.facebook.com/plugins/likebox.php?href=https%3A%2F%2Fwww.facebook.com%2FvPicture&width&height=590&colorscheme=light&show_faces=true&header=true&stream=true&show_border=true&appId=273349419483656"));

}

void MainWindow::shareOnTwitter(){

    QDesktopServices::openUrl(QUrl("https://twitter.com/intent/follow?original_referer=&screen_name=vPicture_&tw_p=followbutton&variant=2.0"));

}

void MainWindow::shareOnGooglePlus(){

    QDesktopServices::openUrl(QUrl("https://plus.google.com/+VpictuRe0"));

}
/*******************/

/***** WINDOW ACTIONS *****/
void MainWindow::minimizeWindow(){
    static int state = 0;

    if(state == 0){
        this->lastPos = this->pos();
        qDebug() << "lastPos: " << this->lastPos << endl;
    }

    if(state == 0){

        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
        QPropertyAnimation *animation2 = new QPropertyAnimation(this, "geometry");
        QPropertyAnimation *animation3 = new QPropertyAnimation(this, "size");
        animation->setDuration(200);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation2->setDuration(300);
        animation2->setStartValue(QRect(this->pos().x(), this->pos().y(), this->width(), this->height()));
        animation2->setEndValue(QRect(this->pos().x(), QApplication::desktop()->height() - 59, this->width(), this->height()));
        animation3->setDuration(100);
        animation3->setStartValue(QSize(this->width(), this->height()));
        animation3->setEndValue(QSize(this->width()/6, this->height()/8));

        animation3->start();
        animation2->start();
        animation->start();
        state ++;

        QTimer::singleShot(400, this, SLOT(minimizeWindow()));
    }else{
        showMinimized();
        state = 0;
    }
}

void MainWindow::showNormalWindow(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
    QPropertyAnimation *animation2 = new QPropertyAnimation(this, "geometry");
    QPropertyAnimation *animation3 = new QPropertyAnimation(this, "size");
    animation->setDuration(600);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation2->setDuration(300);
    animation2->setStartValue(QRect(this->pos().x(), QApplication::desktop()->height() - 60, this->width(), this->height()));
    animation2->setEndValue(QRect(this->lastPos.x(), this->lastPos.y(), this->width(), this->height()));
    animation3->setDuration(100);
    animation3->setStartValue(QSize(this->width()/6, this->height()/8));
    animation3->setEndValue(QSize(this->width(), this->height()));

    animation3->start();
    animation2->start();
    animation->start();
}

/**************************/

/***** NOTIFICATION *****/
void MainWindow::showNotificationPage(){
    // ONLY if there are/is notification in Notification class, and that this class is hidden, we show the notification interface

    if(notification->getNotifications().keys().count()){

        if(secondMainWindow->isHidden()){

            secondMainWindow->showStackedWidgetAt(SecondMainWindow::Notification_Page);

            showProgressivelySecondMainWindow();

        }
    }
}

void MainWindow::showProgressivelySecondMainWindow(){
    static int state = 0;
    if(state == 0){
        secondMainWindow->setWindowOpacity(0.0);
        secondMainWindow->show();
        QPropertyAnimation *animation = new QPropertyAnimation(secondMainWindow, "windowOpacity", secondMainWindow);
        animation->setDuration(700);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start();
        state ++;

        QTimer::singleShot(700, this, SLOT(showProgressivelySecondMainWindow()));
    }else{
        state = 0;
    }
}

void MainWindow::setNotYetSeenNotification(int i){

    if(ui->toolButton_Notification->isHidden())
        ui->toolButton_Notification->show();



    if(i == 0){
        ui->toolButton_Notification->hide();

    }else{

        animateLogoNotification();

        if(i > 9){

            ui->toolButton_Notification->setText("+9");

        }else
            ui->toolButton_Notification->setText(QString::number(i));


    }
}

void MainWindow::animateLogoNotification(){
    static int state = 1;
    if(!ui->toolButton_Notification->isHidden()){
        if(state == 0){

            ui->toolButton_Logo_Notification->setIcon(QIcon(":/Logo/logo_mini_dark"));

            state = 1;
            QTimer::singleShot(850, this, SLOT(animateLogoNotification()));
        }else if(state == 1){

            ui->toolButton_Logo_Notification->setIcon(QIcon(":/Logo/logo_mini"));

            state = 0;
            QTimer::singleShot(850, this, SLOT(animateLogoNotification()));
        }
    }else{

        state = 1;
        ui->toolButton_Logo_Notification->setIcon(QIcon(":/Logo/logo_mini"));
    }
}

void MainWindow::notificationWelcomeMessage(){

    // We send a notification to the user to tell him he is welcome to the vPicture App
    notification->addNotification(Notification::Notif_Notif, tr("Congratulations ! You have installed vPicture App.\nWelcome to the application used by more than 4'000 people in the world. Enjoy and share"), QUrl("http://vpictu.re/gettingstarted"));

}
/**************************/


/********** MENU **********/
void MainWindow::showMenuFor(QWidget *widget){
    m_Menu->listWidget()->clear();
    m_Menu->popup(widget->mapToGlobal(QPoint(0, 0)) + QPoint(0, widget->height() + 3));

    //    if(widget->objectName().toLower().contains("parameters")){
    //        QMenu *menu = new QMenu(tr("Languages"));
    //        menu->setStyleSheet(getFlatMenuDesign());
    //        menu->addActions(config->getLanguagesList());

    //        m_Menu->addMenu(menu);
    //    }else if(widget->objectName().toLower().contains("menu")){

    //    }

    //    m_Menu->addActions(getActionsFor(widget->objectName()));
    //    m_Menu->popup(QPoint(this->pos().x() + widget->pos().x(), this->pos().y() + widget->pos().y() + widget->height()));

}

void MainWindow::showProgressivelyMenu(){
    static int state = 0, done = 0;
    if(state == 0){
        ui->label_menu->hide();
        m_Menu->setWindowOpacity(0.0);
        m_Menu->show();
        m_Menu->listWidget()->clear();
        m_Menu->setArrowSide(MenuMain::Left_Side);

        //        QListWidgetItem *login = new QListWidgetItem(tr("Log in"));
        QListWidgetItem *feedback   = new QListWidgetItem(tr("Feedback"));
        QListWidgetItem *parameters = new QListWidgetItem(tr("Settings"));
        //        QListWidgetItem *help = new QListWidgetItem(tr("Help"));

        //        m_Menu->addItem(login);
        m_Menu->addItem(feedback);
        m_Menu->addItem(parameters);
        //        m_Menu->addItem(help);
        m_Menu->move(ui->toolButton_Menu->mapToGlobal(QPoint(0, 0) - QPoint(15, -(ui->toolButton_Menu->height() + 3))));


        QPropertyAnimation *animation = new QPropertyAnimation(m_Menu, "windowOpacity", m_Menu);
        animation->setDuration(200);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start();


        state ++;

        QTimer::singleShot(700, this, SLOT(showProgressivelyMenu()));
    }else{
        state = 0;
    }
}

void MainWindow::showMenuForLink(){
    static int state = 0, done = 0;
    if(state == 0){
        m_Menu->setWindowOpacity(0.0);
        m_Menu->show();
        m_Menu->listWidget()->clear();
        m_Menu->setArrowSide(MenuMain::Right_Side);

        QListWidgetItem *_default   = new QListWidgetItem(tr("Default"));
        QListWidgetItem *html = new QListWidgetItem("Html");
        QListWidgetItem *bbcode = new QListWidgetItem("BBCode");
        QListWidgetItem *direct = new QListWidgetItem(tr("Direct"));

        m_Menu->addItem(_default);
        m_Menu->addItem(html);
        m_Menu->addItem(bbcode);
        m_Menu->addItem(direct);


        m_Menu->move(ui->toolButton_ArrowAfterUpload->mapToGlobal(QPoint(0, 0) - QPoint(60, -(ui->toolButton_ArrowAfterUpload->height() + 3))));


        QPropertyAnimation *animation = new QPropertyAnimation(m_Menu, "windowOpacity", m_Menu);
        animation->setDuration(200);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start();


        state ++;

        QTimer::singleShot(700, this, SLOT(showMenuForLink()));
    }else{
        state = 0;
    }
}

void MainWindow::itemPressed(QListWidgetItem *item){

    if(item->text() == tr("Log in")){
        qDebug() << "Log in " << endl;
    }else if(item->text() == tr("Settings")){
        secondMainWindow->showStackedWidgetAt(SecondMainWindow::Settings_Page);
        this->showProgressivelySecondMainWindow();
        secondMainWindow->show();
        secondMainWindow->raise();

    }else if(item->text() == tr("Help")){
        this->showProgressivelyOnlineAssitant();
        onlineAssistant->show();
        onlineAssistant->raise();

    }else if(item->text() == tr("Feedback")){
        QDesktopServices::openUrl(QUrl("https://vpicture.uservoice.com/forums/258288-general/filters/top"));

    }else if(item->text() == tr("Default")){

        ui->lineEdit_Url->setText(QString("%{link}").replace("%{link}", serverData->getValue("url").toString()));

        QTimer::singleShot(650, this, SLOT(waitBeforeWidgetAfterUploadShow()));
    }else if(item->text() == "Html"){

        ui->lineEdit_Url->setText(QString("<img src=\"%{direct_link}\" />").replace("%{direct_link}", serverData->getValue("directimg").toString()));

        QTimer::singleShot(650, this, SLOT(waitBeforeWidgetAfterUploadShow()));
    }else if(item->text() == "BBCode"){

        ui->lineEdit_Url->setText(QString("[img]%{direct_link}[/img]").replace("%{direct_link}", serverData->getValue("directimg").toString()));

        QTimer::singleShot(650, this, SLOT(waitBeforeWidgetAfterUploadShow()));
    }else if(item->text() == tr("Direct")){

        ui->lineEdit_Url->setText(QString("%{direct_link}").replace("%{direct_link}", serverData->getValue("directimg").toString()));

        QTimer::singleShot(650, this, SLOT(waitBeforeWidgetAfterUploadShow()));
    }
}
/**************************/

/**** ONLINE ASSISTANT ****/
void MainWindow::showProgressivelyOnlineAssitant(){
    static int state = 0;
    if(state == 0){
        onlineAssistant->setWindowOpacity(0.0);
        onlineAssistant->show();
        QPropertyAnimation *animation = new QPropertyAnimation(onlineAssistant, "windowOpacity", onlineAssistant);
        animation->setDuration(700);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start();
        state ++;

        QTimer::singleShot(700, this, SLOT(showProgressivelyOnlineAssitant()));
    }else{
        state = 0;
    }
}

/**************************/

/***** PICTURES *****/
void MainWindow::showLastPicturePage(){
    secondMainWindow->showStackedWidgetAt(SecondMainWindow::Last_Pictures_Page);
    showProgressivelySecondMainWindow();
}

/*******************/


/***** SETTINGS *****/
void MainWindow::saveWindowPosition(){

    if(settings->getValue("positions/rememberPos").toBool()){

        settings->setValue("positions/wPosX", this->mapToGlobal(QPoint(0, 0)).x());

        settings->setValue("positions/wPosY", this->mapToGlobal(QPoint(0, 0)).y());

        settings->save();

    }
}

/*******************/

/*** INTERNATIONALIZATION ***/
void MainWindow::changeLanguage(QString name, QTranslator *translator){

    localTranslator = translator;

    qApp->removeTranslator(translator);

    translator->load(QString("vpicture_") + name);

    qApp->installTranslator(translator);

    ui->retranslateUi(this);

}

/****************************/

/***** UPDATES *****/
void MainWindow::checkForUpdates(QString version){

    if(version == "none"){
        QVariantMap arguments;
        arguments["v"] = "Alpha";
        url->url("checkforupdates", arguments);

        network->get(url->getUrl());

    }else{

        if(!QCoreApplication::applicationVersion().contains(version, Qt::CaseInsensitive)){

            url->url("", QVariantMap(), "http://vpictu.re/files/vPicture_Alpha_setup.exe");

            network->get(url->getUrl(), Network::Download_Update);

        }
    }
}

void MainWindow::setProgressionUpdates(qint64 bytesReceived, qint64 bytesTotal){
    static int num = 0;
    if(num == 0){
#ifdef Q_OS_WIN
        winTaskBarButton = new QWinTaskbarButton(this);
        winTaskBarButton->setWindow(this->windowHandle());
        winTaskBarButton->setOverlayIcon(QIcon(":/DragAndDrop/draganddrop_valid"));

        winTaskBarProgress = winTaskBarButton->progress();
        winTaskBarProgress->setVisible(true);
#endif
        num ++;
    }
#ifdef Q_OS_WIN
    winTaskBarProgress->setRange(0, bytesTotal);
    winTaskBarProgress->setValue(bytesReceived);
    if(bytesReceived == bytesTotal){
        winTaskBarProgress->setVisible(false);
    }
#endif

    updates->setProgression(bytesReceived, bytesTotal);
}

/*******************/


MainWindow::~MainWindow()
{

    saveWindowPosition();

    settings->save();

    delete ui;
}
