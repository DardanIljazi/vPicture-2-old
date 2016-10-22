#include "secondmainwindow.h"
#include "ui_secondmainwindow.h"


SecondMainWindow::SecondMainWindow(QWidget *parent, Notification *notification, Settings *settings, ServerData *serverData, Network *network, Parser *parser, Config *config) :
    QMainWindow(parent, Qt::WindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)),
    ui(new Ui::SecondMainWindow)
{
    errors            = new Errors;
    
    ui->setupUi(this);
    
    setAttribute(Qt::WA_TranslucentBackground);
    

#ifdef Q_OS_WIN
    // If it is windows, we hide the mac buttons (from the mainwindow.ui)
    ui->widget_TopMacButtons->hide();
#else
    // If it is mac or linux, we stay with mac buttons style
    ui->widget_TopWindowsButtons->hide();
    ui->toolButton_ShareSocial->move(ui->toolButton_ShareSocial->pos().x() - 122, ui->toolButton_ShareSocial->pos().y()+1);
    ui->widgetSocialButtons->move(ui->widgetSocialButtons->pos().x() - 122, ui->widgetSocialButtons->pos().y());
#endif

    
    localNotification = notification;
    
    localSettings     = settings;
    
    localServerData   = serverData;
    
    localNetwork      = network;
    
    localParser       = parser;
    
    localConfig       = config;
    
    localThread       = new Thread;
    
    pressed = false;

    
    ui->listWidget_Settings->item(0)->setSelected(true);
    

    m_signalMapper = new QSignalMapper(this);

    


    scrollBar = ui->listWidget_Pictures->verticalScrollBar();
    
    //    scrollBar->setMaximum(ui->listWidget_Pictures->count()-1);
    //    scrollBar->setValue(0);


    QObject::connect(ui->pushButton_Exit_2, SIGNAL(pressed()), ui->pushButton_Exit, SIGNAL(pressed()));
    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(close()));
    QObject::connect(ui->listWidget_Settings, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(changeStackedWidgetSettings(QListWidgetItem*)));
    QObject::connect(ui->buttonBox_Save_Cancel, SIGNAL(rejected()), this, SLOT(close()));
    QObject::connect(ui->buttonBox_Save_Cancel, SIGNAL(accepted()), this, SLOT(saveSettings()));
    QObject::connect(localParser, SIGNAL(urlImagesReceived(QStringList)), this, SLOT(initiateLinksAndPictures(QStringList)));
    QObject::connect(ui->comboBox_language, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChanged(int)));
    //    QObject::connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(changeItemSelected(int)));


    //    QObject::connect(ui->listWidget_Pictures, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
    QObject::connect(ui->listWidget_Pictures, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(doubleClick(QListWidgetItem*)));
    
}


void SecondMainWindow::showStackedWidgetAt(int i){
    
    if(i == SecondMainWindow::Notification_Page){
        
        initNotificationPage();
        
    }else if(i == SecondMainWindow::Settings_Page){
        
        refreshUiSettings();
        
    }else if(i == SecondMainWindow::Last_Pictures_Page){
        
        localNetworkGetImagesWidhDate();
        
    }
    ui->stackedWidget->setCurrentIndex(i);
    
}

/***** NOTIFICATION *****/
void SecondMainWindow::initNotificationPage(){
    
    QString pathToIcon;
    QListWidgetItem *item;
    for(int i = 0; i < localNotification->getNotifications().keys().count(); i++){
        
        item = new QListWidgetItem(localNotification->getNotifications().values().at(i).first, ui->listWidget);
        
        item->setIcon(QIcon(returnIconPath(localNotification->getNotifications().keys().at(i))));
        
        ui->listWidget->addItem(item);
        
    }
    

    // Set the notification numbers to 0
    setAllSeenNotifications();
    
}

QString SecondMainWindow::returnIconPath(int i){
    if(i == Notification::Errors_Notif)
        return QString(":/Notification/error_icon");
    else if(i == Notification::News_Notif)
        return QString(":/Notification/notification_icon");
    else if(i == Notification::Warning_Notif)
        return QString(":/Notification/warning_icon");
    else
        return QString(":/Notification/notification_icon");
}

void SecondMainWindow::setAllSeenNotifications(bool state){
    
    if(state)
        localNotification->setNotSeenNotification(0, false); // Set the Notification::m_nbNotification var to 0

    
}

/********************/


/***** SETTINGS *****/
void SecondMainWindow::changeStackedWidgetSettings(int i){
    
    ui->stackedWidget_Settings->setCurrentIndex(i);
    
}

void SecondMainWindow::languageChanged(int newLanguage){
}

void SecondMainWindow::changeStackedWidgetSettings(QListWidgetItem *item){
    
    if(item->text() == tr("General")){
         
        ui->stackedWidget_Settings->setCurrentIndex(0);
        
    }else if(item->text() == tr("Screenshot")){
        
        ui->stackedWidget_Settings->setCurrentIndex(1);
        
    }else if(item->text() == tr("GifCreator")){
        
        ui->stackedWidget_Settings->setCurrentIndex(2);
        
    }else if(item->text() == tr("Account")){
        
        ui->stackedWidget_Settings->setCurrentIndex(3);
         
    }
}

void SecondMainWindow::refreshUiSettings(){
    
    /**
      General Stacked Widget*/
    // Set language
    qDebug() << "VALUE: " << localSettings->getValue("software/language") << endl;
    if(localSettings->getValue("software/language") == "fr"){
        ui->comboBox_language->setCurrentIndex(ui->comboBox_language->findText("Français"));
    }else if(localSettings->getValue("software/language") == "it"){
        ui->comboBox_language->setCurrentIndex(ui->comboBox_language->findText("Italiano"));
    }else{
        ui->comboBox_language->setCurrentIndex(ui->comboBox_language->findText("English"));
        localSettings->setValue("software/language", "en");
    }
    // Set link format
    if(localSettings->getValue("software/linkFormat").toString() == "%{link}"){
        ui->comboBox_linksformat->setCurrentIndex(ui->comboBox_linksformat->findText(tr("Default")));
    }else if(localSettings->getValue("software/linkFormat").toString() == "<img src=\"%{direct_link}\" />"){
        ui->comboBox_linksformat->setCurrentIndex(ui->comboBox_linksformat->findText("Html"));
    }else if(localSettings->getValue("software/linkFormat").toString() == "[img]%{direct_link}[/img]"){
        ui->comboBox_linksformat->setCurrentIndex(ui->comboBox_linksformat->findText("BBCode"));
    }else if(localSettings->getValue("software/linkFormat").toString() == "%{direct_link}"){
        ui->comboBox_linksformat->setCurrentIndex(ui->comboBox_linksformat->findText(tr("Direct")));
    }else{
        localSettings->setValue("software/linkFormat", "Default");
    }
    // Set Detect Screen key
    ui->checkBox_DetectScreenKey->setChecked(localSettings->getValue("software/detectPrintScreenKey").toBool());
    // Set Remember window position
    ui->checkBox_RememberWindowPosition->setChecked(localSettings->getValue("positions/rememberPos").toBool());
    /**
    ***********************************/

    /**
      Screenshot Stacked Widget*/
    // Detect window on screen
    ui->checkBox_detectWindowOnScreen->setChecked(localSettings->getValue("screenshot/useWid").toBool());
    /**
    ***********************************/

    /**
      GifCreator Stacked Widget*/
    // File Per Second
    ui->spinBox_Fps->setValue(localSettings->getValue("gifcreator/filePerSecond").toInt());
    // Show Cursor on animation
    ui->checkBox_ShowCursor->setChecked(localSettings->getValue("gifcreator/showCursor").toBool());
    /**
    ***********************************/

    /**
      Account Stacked Widget*/
    // Account name
    ui->label_pseudonyme->setText(localServerData->getValue("username").toString());
    // Contact Email
    ui->label_email->setText(localServerData->getValue("email").toString());
    // Security Level
    ui->label_securityLevel->setText(localServerData->getValue("securityLevel").toString());
    /**
    ***********************************/
    
}

void SecondMainWindow::updateSettings(){
    
    /**
      General Stacked Widget*/
    // Set language
    int index = ui->comboBox_language->currentIndex();
    if(index == 0){
        localSettings->setValue("software/language", "en");
        qApp->removeTranslator(&translator);
        ui->retranslateUi(this);
        ui->comboBox_language->setCurrentIndex(0);
        emit languageChanged_("en", &translator);

    }else if(index == 1){
        localSettings->setValue("software/language", "fr");
        qApp->removeTranslator(&translator);
        translator.load(QString("vpicture_fr"));
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
        ui->comboBox_language->setCurrentIndex(1);
        emit languageChanged_("fr", &translator);

    }else if(index == 2){
        localSettings->setValue("software/language", "it");
        qApp->removeTranslator(&translator);
        translator.load(QString("vpicture_it"));
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
        ui->comboBox_language->setCurrentIndex(2);
        emit languageChanged_("it", &translator);
    }

    // Set link format
    if(ui->comboBox_linksformat->currentText().contains("BBCode")){
        localSettings->setValue("software/linkFormat", "[img]%{direct_link}[/img]");
    }else if(ui->comboBox_linksformat->currentText().contains("Html")){
        localSettings->setValue("software/linkFormat", "<img src=\"%{direct_link}\" />");
    }else if(ui->comboBox_linksformat->currentText().contains("Direct")){
        localSettings->setValue("software/linkFormat", "%{direct_link}");
    }else{
        localSettings->setValue("software/linkFormat", "%{link}");
    }
    // Set Detect Screen key
    localSettings->setValue("software/detectPrintScreenKey", ui->checkBox_DetectScreenKey->isChecked());
    // Set Remember window position
    localSettings->setValue("positions/rememberPos", ui->checkBox_RememberWindowPosition->isChecked());
    /**
    ***********************************/

    /**
      Screenshot Stacked Widget*/
    // Detect window on screen
    localSettings->setValue("software/detectPrintScreenKey", ui->checkBox_detectWindowOnScreen->isChecked());
    /**
    ***********************************/

    /**
      GifCreator Stacked Widget*/
    // File Per Second
    localSettings->setValue("gifcreator/filePerSecond", ui->spinBox_Fps->value());
    // Show Cursor on animation
    localSettings->setValue("gifcreator/showCursor", ui->checkBox_ShowCursor->isChecked());
    /**
    ***********************************/

    /**
      Account Stacked Widget*/
    // Account name
    ui->label_pseudonyme->setText(localServerData->getValue("username").toString());
    // Contact Email
    ui->label_email->setText(localServerData->getValue("email").toString());
    // Security Level
    ui->label_securityLevel->setText(localServerData->getValue("securityLevel").toString());
    /**
    ***********************************/
    
    localSettings->save();
    
}

void SecondMainWindow::saveSettings(){
    
    updateSettings();
    
    this->close();
    
}

/********************/


/***** PICTURES *****/

void SecondMainWindow::localNetworkGetImagesWidhDate(){
    
    localNetwork->getImagesWithDate();
    
}

void SecondMainWindow::initiateLinksAndPictures(QStringList links){
    //    localThread->deleteListDuplicates(getCurrentLinksInListWidget(), &links);
    
    if(!links.isEmpty()){
        
        picturesLinks.append(links);
        

        QFuture<void> future = QtConcurrent::run(this, &SecondMainWindow::addItems);
        
    }
}

void SecondMainWindow::addItems(){
    
    ui->listWidget_Pictures->clear();
    
    for(int i = 0; i < picturesLinks.count(); i ++){
        ui->listWidget_Pictures->addItem(picturesLinks.at(i));
    }
    
    picturesLinks.clear();
}

void SecondMainWindow::procedeSignalMapper(QString string){
    if(string.contains("$pushbutton_link$")){
        openLink(string.replace("$pushbutton_link$", ""));
    }else if(string.contains("$pushbutton_copy$")){
        indexOfLastCopyButtonPressed.append(getCurrentLinksInListWidget().indexOf(string.replace("$pushbutton_copy$", "")));
        copyButtons.at(indexOfLastCopyButtonPressed.last())->setText(tr("Copied!"));
        copyButtons.at(indexOfLastCopyButtonPressed.last())->setStyleSheet("QPushButton{"
                                                                           "border: 1px solid #2ecc71;"
                                                                           "min-width: 50px;"
                                                                           "padding: 3px;"
                                                                           "background-color: #2ecc71;"
                                                                           "color: #ecf0f1;"
                                                                           "font: 8pt \"Open Sans\";"
                                                                           "}"
                                                                           "QPushButton::hover{"
                                                                           "background-color: #27ae60;"
                                                                           "}");

        QTimer::singleShot(2500, this, SLOT(setPushButtonTextToNormalState()));
        emit copyLink(string.replace("$pushbutton_copy$", ""));
    }
}

void SecondMainWindow::setPushButtonTextToNormalState(){
    /*copyButtons.at(indexOfLastCopyButtonPressed)->setText(tr("Copy link"));
    copyButtons.at(indexOfLastCopyButtonPressed)->setStyleSheet("QPushButton{"
                                                                "border: 1px solid #34495e;"
                                                                "min-width: 50px;"
                                                                "padding: 3px;"
                                                                "background-color: #2c3e50;"
                                                                "color: #ecf0f1;"
                                                                "font: 8pt \"Open Sans\";"
                                                                "}"
                                                                "QPushButton::hover{"
                                                                "background-color: #34495e;"
                                                                "}");*/
}

void SecondMainWindow::addButtons(QString link, int i, QListWidgetItem *item){

}

void SecondMainWindow::openLink(QString link){
    
    QDesktopServices::openUrl(QUrl(link));
    
}
// Obsolete, soon used in the Thread
QString SecondMainWindow::getHttpLocalFilePath(QString httpLink){
    QUrl url(httpLink);
    QFile file;

    for(int i = 0; i < localConfig->getListMimeDataAllowed().count(); i++){
        file.setFileName(QString("pictures") + url.path() + QString(".") + localConfig->getListMimeDataAllowed().at(i));
        qDebug() << file.fileName() << endl;
        if(file.exists())
            return file.fileName();
    }

    return ":/ListWidget/transparent_picture";
}

void SecondMainWindow::changeItemSelected(int val){
    
    if(val == scrollBar->maximum()){
        localNetwork->getImagesWithDate();
    }
    
}

QStringList SecondMainWindow::getCurrentLinksInListWidget(){
    QStringList list;
    for(int i = 0; i < ui->listWidget_Pictures->count(); i ++){
        list << ui->listWidget_Pictures->item(i)->whatsThis();
    }

    return list;
}

void SecondMainWindow::doubleClick(QListWidgetItem *item){
    QDesktopServices::openUrl(QUrl(item->text()));
}


/********************/


void SecondMainWindow::mouseMoveEvent(QMouseEvent *event){
    if(ui->label_Background_border->underMouse() && pressed && this->underMouse()){
        QPoint p = event->globalPos();
        window()->move(p - m_lastPoint);
    }
}

void SecondMainWindow::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void SecondMainWindow::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->label_Background_border->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
    }
}

void SecondMainWindow::closeEvent(QCloseEvent *event){
    static int state = 0, response = -1;
    if(state == 0){
        //        updateSettings();
        //        settingsEndImage = localSettings->getSettingsVariant();

        //        if(settingsBeginImage != settingsEndImage){
        //            Dialog *dialog = new Dialog();
        //            dialog->buttons()->setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
        //            response = dialog->box(0, tr("Settings"), tr("Your settings have been changed.<br />Do you want to exit without saving ?"), true);
        //            qDebug() << "response: " << response << endl;
        //        }
        state ++;
        if(response == 1 || response == -1){
            QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
            animation->setDuration(350);
            animation->setStartValue(1.0);
            animation->setEndValue(0.0);
            animation->start();
        }

        event->ignore();
        QTimer::singleShot(400, this, SLOT(close()));
    }else{
        state = 0;
        if(response == 1 || response == -1){
            response = -1;
            event->accept();
        }else{
            response = -1;
            event->ignore();
        }
    }
}


void SecondMainWindow::showEvent(QShowEvent *event){
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}

SecondMainWindow::~SecondMainWindow(){
    
    delete ui;
}
