#include "../Core/mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::event(QEvent *event){
    //    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);

    //    ui->label_menu->hide();
    //    ui->label_parameters->hide();
    //    ui->label_screenshot->hide();

    //    if(event->type() == QEvent::MouseMove){
    //        if(ui->toolButton_Menu->underMouse()){
    //            ui->label_menu->show();
    //        }else if(ui->toolButton_Parameters->underMouse()){
    //            ui->label_parameters->show();
    //        }else if(ui->toolButton_Screenshot->underMouse()){
    //            ui->label_screenshot->show();
    //        }
    //        qDebug() << mouseEvent->pos() << endl;
    //    }

    return QMainWindow::event(event);
}

void MainWindow::leaveEvent(QEvent *){
    if(!this->underMouse()){
        changeMiddlePictureState(MainWindow::None_File);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){

    if(!this->cornered){
        if((ui->label_Background_border->underMouse() && pressed && this->underMouse()) || (ui->label_vPictue_icon->underMouse() && pressed)){
            QPoint p = event->globalPos();
            window()->move(p - m_lastPoint);
        }
        if(!ui->widgetSocialButtons->isHidden()){
            if(!ui->toolButton_ShareSocial->underMouse() && !ui->widgetSocialButtons->underMouse()
                    && (!ui->facebook2->underMouse() || !ui->twitter2->underMouse() || !ui->googleplus2->underMouse())){
                if(ui->widgetSocialButtons->height() == 65)
                    hideWidgetSocialButtons();
            }
        }
    }


    ui->label_menu->hide();
    //    ui->label_parameters->hide();
    ui->label_screenshot->hide();
    ui->label_pictures->hide();

    if(ui->toolButton_Menu->underMouse()){
        if(m_Menu->isHidden() || m_Menu->windowOpacity() == 0.0)
            ui->label_menu->show();
    }/* else if(ui->toolButton_Parameters->underMouse()){
        ui->label_parameters->show();
    }*/else if(ui->toolButton_Screenshot->underMouse()){
        ui->label_screenshot->show();
    }else if(ui->toolButton_Pictures->underMouse()){
        ui->label_pictures->show();
    }

    if(pressed){
        if(isOutOfScreen()){
            this->cornered = true;
            miniOutWidget->show();
        }
        else{
            this->cornered = false;
            miniOutWidget->hide();
        }
    }
}

bool MainWindow::isOutOfScreen(){
    //    qDebug() << "QPoint: "  << this->geometry().x() + ui->label_Background_border->pos().x() << " " << ui->label_Background_border->width() << " " << -((ui->label_Background_border->width())/100*75) << endl;
    // left side
    if(this->geometry().x() + ui->label_Background_border->pos().x() < -((ui->label_Background_border->width())/100*30)){
        setOnLeftCorner();
        return true;
    }
    // Right side
    else if(this->geometry().x() + ui->label_Background_border->geometry().right() > (QApplication::desktop()->width() + (ui->label_Background_border->width())/100*50)){
        setOnRightCorner();
        return true;
    }

    return false;
}

void MainWindow::setOnLeftCorner(){
    miniOutWidget->move(0, this->pos().y());
    this->move(-(ui->label_Background_border->geometry().right()) - 1, this->pos().y());
    //    this->cursor().setPos(miniOutWidget->width() - 5, this->cursor().pos().y());
}

void MainWindow::setOnRightCorner(){
    miniOutWidget->move(QApplication::desktop()->width() - miniOutWidget->width(), this->pos().y());
    this->move(QApplication::desktop()->width() + ui->label_Background_border->pos().x(), this->pos().y());
    //    this->cursor().setPos(QApplication::desktop()->width() - miniOutWidget->width() + 5, this->cursor().pos().y());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->label_Background_border->underMouse() || ui->label_vPictue_icon->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
        this->setFocus();
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    static int state = 0;
    if(state == 0){
        QApplication::clipboard()->clear();
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
        animation->setDuration(150);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->start();

        state ++;
        event->ignore();
        QTimer::singleShot(150, this, SLOT(close()));
    }else{
        event->accept();
        qApp->exit();
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}


void MainWindow::changeEvent(QEvent *event){
    /***** INTERNATIONALIZATION (LANGUAGES) *****/
    if (event->type() == QEvent::LanguageChange) {

    }
    /********************************************/
    if (event->type() == QEvent::WindowStateChange)
    {
        //        QWindowStateChangeEvent *stateEv = static_cast<QWindowStateChangeEvent*>(event);
        if(this->windowState() == Qt::WindowNoState){
            showNormalWindow();
        }

    }
    return QMainWindow::changeEvent(event);
}

