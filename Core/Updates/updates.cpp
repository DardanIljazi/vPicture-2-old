#include "updates.h"
#include "ui_updates.h"

Updates::Updates(QWidget *parent) :
    QWidget(parent, Qt::WindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool)),
    ui(new Ui::Updates)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);

    localErrors = new Errors;
    launcherProcess = new QProcess;

#ifdef Q_OS_WIN
    // If it is windows, we hide the mac buttons (from the mainwindow.ui)
    ui->widget_TopMacButtons->hide();
#else
    // If it is mac or linux, we stay with mac buttons style
    ui->widget_TopWindowsButtons->hide();
    ui->toolButton_ShareSocial->move(ui->toolButton_ShareSocial->pos().x() - 122, ui->toolButton_ShareSocial->pos().y()+1);
    ui->widgetSocialButtons->move(ui->widgetSocialButtons->pos().x() - 122, ui->widgetSocialButtons->pos().y());
#endif

    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(close()));
    QObject::connect(launcherProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(launchSoft(int,QProcess::ExitStatus)));
    QObject::connect(launcherProcess, SIGNAL(started()), this, SLOT(close()));
}

void Updates::mouseMoveEvent(QMouseEvent *event){
    if((ui->label_Background_border->underMouse() && pressed && this->underMouse()) || (ui->label_title->underMouse() && pressed)){
        QPoint p = event->globalPos();
        window()->move(p - m_lastPoint);
    }
}

void Updates::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void Updates::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->label_Background_border->underMouse() || ui->label_title->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
    }
}

void Updates::closeEvent(QCloseEvent *event){
    static int state = 0;
    if(ui->progressBar_Upload->value() == ui->progressBar_Upload->maximum()){
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
    }else{
        event->ignore();
    }
}

void Updates::showEvent(QShowEvent *event){
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}

void Updates::setProgression(qint64 bytesReceived, qint64 bytesTotal){
    this->show();
    ui->progressBar_Upload->show();
    if (bytesTotal != 0)
    {
        ui->progressBar_Upload->setRange(0, bytesTotal);
        ui->progressBar_Upload->setValue(bytesReceived);
    }
}

void Updates::saveAndLaunchSetup(QByteArray dataSetupLauncher){
    QFile::remove("vPicture_Alpha_setup.exe");
    QFile setup("vPicture_Alpha_setup.exe");
    if (setup.open(QFile::WriteOnly)) {
        setup.write(dataSetupLauncher);
        setup.close();
    }else{
        local
    }

    QTimer::singleShot(2500, this, SLOT(launchSoft()));
}

void Updates::launchSoft(int exitCode, QProcess::ExitStatus exitStatus){
    static int code = 0;
    if(code == 0){
        launcherProcess->start("vPicture_Alpha_setup.exe");
        code++;
    }else{
        if(exitCode != -10000){
            // Finished
            local
            code ++;

        }else{
//            if(code >= 3){
//                local
//            }else{
//                code ++;
//                QTimer::singleShot(2500, this, SLOT(launchSoft()));
//            }
        }
    }
}


Updates::~Updates()
{
    delete ui;
}
