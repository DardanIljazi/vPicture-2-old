#include "onlineassistant.h"
#include "ui_onlineassistant.h"

OnlineAssistant::OnlineAssistant(QWidget *parent) :
    QWidget(parent, Qt::WindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)),
    ui(new Ui::OnlineAssistant)
{
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

    QObject::connect(ui->toolButton_Premium, SIGNAL(pressed()), this, SLOT(openPremiumUrl()));
    QObject::connect(ui->toolButton_Security, SIGNAL(pressed()), this, SLOT(openSecurityUrl()));
    QObject::connect(ui->toolButton_Online_Help, SIGNAL(pressed()), this, SLOT(openOnlineHelpWidget()));
    QObject::connect(ui->toolButton_Upload, SIGNAL(pressed()), this, SLOT(openUploadUrl()));
    QObject::connect(ui->toolButton_Screenshot, SIGNAL(pressed()), this, SLOT(openScreenshotUrl()));
    QObject::connect(ui->toolButton_Gif_Creator, SIGNAL(pressed()), this, SLOT(openGifCreatorUrl()));
    QObject::connect(ui->toolButton_Image_Editor, SIGNAL(pressed()), this, SLOT(openImageEditorUrl()));
    QObject::connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(detectHowManyChar()));

    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(close()));

    QObject::connect(ui->pushButton_Cancel, SIGNAL(pressed()), this, SLOT(changeStackedWidgetToMain()));
}

void OnlineAssistant::changeStackedWidgetToMain(){
    ui->stackedWidget->setCurrentIndex(0);
}

void OnlineAssistant::openPremiumUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/premium"));
}

void OnlineAssistant::openSecurityUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/help/security"));
}

void OnlineAssistant::openOnlineHelpWidget(){
    ui->stackedWidget->setCurrentIndex(1);
}

void OnlineAssistant::openUploadUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/help/security"));
}

void OnlineAssistant::openScreenshotUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/help/screenshot"));
}

void OnlineAssistant::openGifCreatorUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/help/gifcreator"));
}

void OnlineAssistant::openImageEditorUrl(){
    QDesktopServices::openUrl(QUrl("http://vpictu.re/help/imageeditor"));
}

void OnlineAssistant::mouseMoveEvent(QMouseEvent *event){
    if(ui->label_Background_border->underMouse() && pressed && this->underMouse()){
        QPoint p = event->globalPos();
        window()->move(p - m_lastPoint);
    }
}

void OnlineAssistant::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void OnlineAssistant::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->label_Background_border->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
    }
}

void OnlineAssistant::detectHowManyChar(){
    if(ui->textEdit->toPlainText().length() > 253){
        ui->textEdit->textCursor().deletePreviousChar();
    }
    ui->label_wordsNumber->setText(QString::number(253 - ui->textEdit->toPlainText().length()));
}

void OnlineAssistant::closeEvent(QCloseEvent *event){
    static int state = 0;
    if(state == 0){
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
        animation->setDuration(350);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->start();

        state ++;
        event->ignore();
        QTimer::singleShot(400, this, SLOT(close()));
    }else{
        state = 0;
        event->accept();
    }

}

void OnlineAssistant::showEvent(QShowEvent *event){
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}

OnlineAssistant::~OnlineAssistant()
{
    delete ui;
}
