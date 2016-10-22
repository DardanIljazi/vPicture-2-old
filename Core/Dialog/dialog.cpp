#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, QString title, QString text) :
    QDialog(parent, Qt::WindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool)),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    returnValue = -1;

#ifdef Q_OS_WIN
    // If it is windows, we hide the mac buttons (from the mainwindow.ui)
    ui->widget_TopMacButtons->hide();
#else
    // If it is mac or linux, we stay with mac buttons style
    ui->widget_TopWindowsButtons->hide();
    ui->toolButton_ShareSocial->move(ui->toolButton_ShareSocial->pos().x() - 122, ui->toolButton_ShareSocial->pos().y()+1);
    ui->widgetSocialButtons->move(ui->widgetSocialButtons->pos().x() - 122, ui->widgetSocialButtons->pos().y());
#endif

    if(!title.isEmpty() && !text.isEmpty())
        box(parent, title, text);

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(reject()));
}

int Dialog::box(QWidget *parent, QString title, QString text, bool blockUntil){
    QEventLoop loop;

    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(setReturnValue(int)));
    QObject::connect(this, SIGNAL(finished(int)), &loop, SLOT(quit()));

    this->setParent(parent);
    ui->label_title->setText(title);
    ui->textEdit->setText(text);
    this->show();
    if(blockUntil)
        loop.exec();

    return returnButtonValue();
}

QDialogButtonBox *Dialog::buttons(){
    return ui->buttonBox;
}

void Dialog::setReturnValue(int value){
    returnValue = value;
}

int Dialog::returnButtonValue(){
    return returnValue;
}

void Dialog::mouseMoveEvent(QMouseEvent *event){
    if((ui->label_Background_border->underMouse() && pressed && this->underMouse()) || (ui->label_title->underMouse() && pressed)){
        QPoint p = event->globalPos();
        window()->move(p - m_lastPoint);
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void Dialog::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->label_Background_border->underMouse() || ui->label_title->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
    }
}

void Dialog::closeEvent(QCloseEvent *event){
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

void Dialog::showEvent(QShowEvent *event){
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}

Dialog::~Dialog()
{
    delete ui;
}
