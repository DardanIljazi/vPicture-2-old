#include "gifcreator.h"
#include "ui_gifcreator.h"

GifCreator::GifCreator(QWidget *parent, Settings *settings) :
    QWidget(parent, Qt::WindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)),
    ui(new Ui::GifCreator)
{
    setAttribute(Qt::WA_TranslucentBackground);
    ui->setupUi(this);

    localSettings = settings;

    stopAction = false;
    numbPixmap = 0;
    tempName = "animate";
    pressed = false;
    pressedOnCorner = false;

    ui->widget_Photo->hide();

    ui->spinBox_Fps->setValue(localSettings->getValue("gifcreator/filePerSecond").toInt());

    msBeforeNewScreen = (1000 /ui->spinBox_Fps->value());
    ui->lineEdit_Width->setValidator(new QIntValidator(10, QApplication::desktop()->width() + ui->widget_borderLeft->width() + ui->widget_borderRight->width(), this));
    ui->lineEdit_Height->setValidator(new QIntValidator(10, QApplication::desktop()->height() - ui->Bottom_Bar->height(), this));
    this->setMaximumHeight(QApplication::desktop()->height());


    QObject::connect(ui->spinBox_Fps, SIGNAL(valueChanged(int)), this, SLOT(setMsBetweenEachScreen(int)));
    //    QObject::connect(ui->lineEdit_Width, SIGNAL(valueChanged(int)), this, SLOT(resizeWidth(int)));
    //    QObject::connect(ui->lineEdit_Height, SIGNAL(valueChanged(int)), this, SLOT(resizeHeight(int)));
    QObject::connect(ui->pushButton_Pause, SIGNAL(pressed()), this, SLOT(pause()));
    QObject::connect(ui->pushButton_Record, SIGNAL(pressed()), this, SLOT(start()));
    QObject::connect(ui->pushButton_Stop, SIGNAL(pressed()), this, SLOT(stop()));
    QObject::connect(ui->pushButton_Exit, SIGNAL(pressed()), this, SLOT(close()));
}

void GifCreator::resizeHeight(int height){
    ui->widget_Photo->resize(ui->widget_Photo->width(), height);
}

void GifCreator::resizeWidth(int width){
    ui->widget_Photo->resize(width, ui->widget_Photo->height());
}

void GifCreator::start(){
    //    this->cursor().pixmap().save("blabla.png", "PNG");
    stopAction = false;
    ui->widget_Photo->hide();
    ui->pushButton_Record->setStyleSheet("QPushButton{ border: none; image: url(:/GifCreator/record_active);}QPushButton:hover{image: url(:/GifCreator/record_hover);}");
    ui->pushButton_Stop->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/stop);} QPushButton:hover{image: url(:/GifCreator/stop_hover);}");
    ui->pushButton_Pause->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/pause); } QPushButton:hover{image: url(:/GifCreator/pause_hover);}");

    QTimer::singleShot(450, this, SLOT(process()));
}

void GifCreator::process(){
    if(!stopAction){
        ui->widget_Photo->hide();
        screen = QGuiApplication::primaryScreen();
        desktopScreen = screen->grabWindow(0);
        QPainter p(&desktopScreen);
        if(localSettings->getValue("gifcreator/showCursor").toBool()){
#ifdef Q_OS_WIN
            p.drawPixmap(this->cursor().pos(screen), QPixmap(":/Cursor/windows_cursor"), QPixmap(":/Cursor/windows_cursor").rect());
#else
            p.drawPixmap(this->cursor().pos(screen), QPixmap(":/Cursor/mac_cursor"), QPixmap(":/Cursor/windows_cursor").rect());
#endif
            p.end();
        }

        msBeforeNewScreen = (1000 / ui->spinBox_Fps->value());
        rectScreen = desktopScreen.copy(this->geometry().x() + ui->widget_borderLeft->width(), this->geometry().y() + ui->widget_Top->height(), ui->widget_Photo->width(), ui->widget_Photo->height());

        saveToList();
    }
}

void GifCreator::pause(){
    //    ui->widget_Photo->show();
    ui->pushButton_Stop->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/stop);} QPushButton:hover{image: url(:/GifCreator/stop_hover);}");
    ui->pushButton_Record->setStyleSheet("QPushButton{ border: none; image: url(:/GifCreator/play);}QPushButton:hover{image: url(:/GifCreator/play_hover);}");
    ui->pushButton_Pause->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/pause_active); } QPushButton:hover{image: url(:/GifCreator/pause_hover);}");
    ui->label_nbPictures->setText(ui->label_nbPictures->text().replace("Recording: ", "Paused: "));
    stopAction = true;
}

void GifCreator::deblockPause(){
    ui->pushButton_Record->setStyleSheet("QPushButton{ border: none; image: url(:/GifCreator/record);}QPushButton:hover{image: url(:/GifCreator/record_hover);}");
    ui->pushButton_Stop->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/stop);} QPushButton:hover{image: url(:/GifCreator/stop_hover);}");
    ui->pushButton_Pause->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/pause); } QPushButton:hover{image: url(:/GifCreator/pause_hover);}");
    stopAction = false;
    process();
}

void GifCreator::closeEvent(QCloseEvent *event){
    static int state = 0;
    if(state == 0){
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
        animation->setDuration(250);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->start();

        state ++;
        event->ignore();
        QTimer::singleShot(300, this, SLOT(close()));
    }else{
        state = 0;
        stopAction = true;
        event->accept();
    }
}

void GifCreator::mouseMoveEvent(QMouseEvent *event){
    if(pressedOnCorner){
        if(getCornerSelected() == GifCreator::top_left){
            this->setGeometry(QRect(this->geometry().bottomRight(), event->globalPos()).normalized());
        }else if(getCornerSelected() == GifCreator::top_right){
            this->setGeometry(QRect(this->geometry().bottomLeft(), event->globalPos()).normalized());
        }else if(getCornerSelected() == GifCreator::bottom_right){
            this->setGeometry(QRect(this->geometry().topLeft(), event->globalPos()).normalized());
        }else if(getCornerSelected() == GifCreator::bottom_left){
            this->setGeometry(QRect(this->geometry().topRight(), event->globalPos()).normalized());
        }else if(getCornerSelected() == GifCreator::middle_left){
            this->setGeometry(QRect(this->geometry().bottomRight(), QPoint(this->geometry().x() + (event->globalPos().x() - this->geometry().x()), this->geometry().y())).normalized());
        }else if(getCornerSelected() == GifCreator::middle_right){
            this->setGeometry(QRect(this->geometry().bottomLeft(),  QPoint(this->geometry().x()+ (event->globalPos().x()- this->geometry().x()), this->geometry().y())).normalized());
        }else if(getCornerSelected() == GifCreator::top_middle){
            this->setGeometry(QRect(this->geometry().bottomRight(), QPoint(this->geometry().x(), this->geometry().y() + (event->globalPos().y() - this->geometry().y()))).normalized());
        }else if(getCornerSelected() == GifCreator::bottom_middle){
            this->setGeometry(QRect(this->geometry().topRight(),    QPoint(this->geometry().x(), this->geometry().y() + (event->globalPos().y() - this->geometry().y()))).normalized());
        }
        update();
    }

    if(!isMouseOnWindowCorner(event)){
        setCursor(Qt::ArrowCursor);
    }

    if(!pressedOnCorner){
        if(ui->widget_Top->underMouse() && pressed && this->underMouse()){
            QPoint p = event->globalPos();
            window()->move(p - m_lastPoint);
        }
    }
}

void GifCreator::mouseReleaseEvent(QMouseEvent *){
    pressed = false;
    pressedOnCorner = false;
}

void GifCreator::mousePressEvent(QMouseEvent *mouseEvent){
    if(ui->widget_Top->underMouse()){
        pressed = true;
        m_lastPoint = mouseEvent->pos();
    }

    if(isMouseOnWindowCorner(mouseEvent)){
        pressedOnCorner = true;
    }
}

void GifCreator::saveToList(){
    static int i = 0;
    if(i == 0){
        if(numbPixmap == 0){
            QDateTime time = QDateTime::currentDateTime();
            QString path = QString("animations/%1/").arg(time.toString("ddMMyy_hh_mm"));
            pathToPictures = QFileInfo(path).path();
            dir = QDir(path);
            dir.mkpath(".");
        }
        i++;
    }

    listPngToSave.append(rectScreen);

    threadSave();

    numbPixmap ++;
    ui->label_nbPictures->clear();
    ui->label_nbPictures->setText(QString("<span style='color: #e74c3c;'>") + tr("Recording: ") + QString("</span>") + QString::number(numbPixmap) + QString(" ") + tr("files"));
    QTimer::singleShot(msBeforeNewScreen, this, SLOT(process()));
}

void GifCreator::threadSave(){
    QFuture<void> future = QtConcurrent::run(this, &GifCreator::save);
}

void GifCreator::save(){
    for(; !listPngToSave.isEmpty(); ){
        QFile file(dir.path() + "/" + tempName + QString("_") + QString("%1").arg(numbPixmap, 5, 10, QChar('0')).toUpper() + QString(".png"));
        file.open(QIODevice::WriteOnly);
        if(!listPngToSave.isEmpty()){
            listPngToSave.at(0).save(&file, "PNG");
            listPngToSave.takeAt(0);
        }

        file.close();
    }
}

void GifCreator::stop(){
    ui->pushButton_Record->setStyleSheet("QPushButton{ border: none; image: url(:/GifCreator/record);}QPushButton:hover{image: url(:/GifCreator/record_hover);}");
    ui->pushButton_Stop->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/stop);} QPushButton:hover{image: url(:/GifCreator/stop_hover);}");
    ui->pushButton_Pause->setStyleSheet("QPushButton{border: none; image: url(:/GifCreator/pause); } QPushButton:hover{image: url(:/GifCreator/pause_hover);}");
    stopAction = true;
    if(numbPixmap > 0)
        convertToGif();

    numbPixmap = 0;
}

void GifCreator::convertToGif(){
    convertImageMagick = new QProcess();
    QStringList arguments;
    arguments << "-delay" << "20" << "   " << "-loop" << "0" <<  "   " << QString(QCoreApplication::applicationDirPath() + QString("/") + pathToPictures + QString("/animate_*.png")) << "   " << QString(QCoreApplication::applicationDirPath() + QString("/") + pathToPictures + QString("/result.gif"));
    qDebug() << arguments << endl;
#ifdef Q_OS_WIN
    convertImageMagick->start("convert.exe", arguments);

    QByteArray result = convertImageMagick->readAll();
    qDebug() << "result = " << result << endl;
#endif
    QObject::connect(convertImageMagick, SIGNAL(readyReadStandardError()), this, SLOT(showImageMagickConvertErrors()));

    saveGif();
}

void GifCreator::showImageMagickConvertErrors(){
//    Dialog *dialog = new Dialog(0, tr("Error"), convertImageMagick->readAllStandardError());
//    dialog->raise();
//    dialog->show();
}

void GifCreator::saveGif(){
    waitGifAnimatedFinished();
}

void GifCreator::waitGifAnimatedFinished(){
    static int i = 0;
    if(isGifAnimatedFinished()){
        ui->label_Working->setStyleSheet("color: #16a085;");
        ui->label_Working->setText(tr("Done!"));
        QString format = "gif";
        QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
                                                        tr("%1 Files (*.%2);;All Files (*)")
                                                        .arg(format.toUpper())
                                                        .arg(format));

        QFile file(QString(QCoreApplication::applicationDirPath() + QString("/") + pathToPictures + QString("/result.gif")));
        file.copy(fileName);
        file.close();

        QTimer::singleShot(1500, this, SLOT(callGifEditor()));
    }else{
        if(i < 3){
            ui->widget_Photo->show();
            ui->label_Working->setStyleSheet("color: #16a085;");
            ui->label_Working->setText(tr("Working"));
            ui->label_Working->show();
            if(ui->widget_Photo->height() < ui->widget_Photo->width())
                ui->label_Working->setFont(QFont("Open Sans", (ui->widget_Photo->height()/1.61803398875)/4 ));
            else
                ui->label_Working->setFont(QFont("Open Sans", (ui->widget_Photo->width()/1.61803398875)/4 ));

            ui->label_Working->setText(ui->label_Working->text() + QString("."));
            i ++;
        }else{
            ui->label_Working->setText(ui->label_Working->text().replace("...", ""));
            i = 0;
        }
        QTimer::singleShot(1000, this, SLOT(waitGifAnimatedFinished()));
    }
}

bool GifCreator::isGifAnimatedFinished(){
    QFile file( QString(pathToPictures) + QString("/result.gif"));
    if (!file.open(QIODevice::ReadOnly)) return false;

    QByteArray data = file.readAll();
    file.close();

    if( (data.toHex().toLower().at(data.toHex().length()-4) == QChar('0')
         && data.toHex().toLower().at(data.toHex().length()-3) == QChar('0')
         && data.toHex().toLower().at(data.toHex().length()-2) == QChar('3')
         && data.toHex().toLower().at(data.toHex().length()-1) == QChar('b') )
            ||
            (data.toHex().toLower().at(data.toHex().length()-3) == QChar('0')
             && data.toHex().toLower().at(data.toHex().length()-2) == QChar('0')
             && data.toHex().toLower().at(data.toHex().length()-1) == QChar('3')
             && data.toHex().toLower().at(data.toHex().length()) == QChar('b') ) )
    {
        return true;
    }


    return false;
}

void GifCreator::setRect(QRect rect){
    lastRect = rect;
    ui->widget_Photo->resize(rect.size());
    this->move(rect.x() - ui->widget_borderLeft->width(), rect.y() - ui->widget_Top->height());
    if(this->geometry().y() < 0){
        this->move(this->pos().x(), 0);
    }
    this->resize(ui->widget_Photo->size().width() + ui->widget_borderLeft->width() + ui->widget_borderRight->width(), ui->widget_Photo->size().height() + ui->widget_Top->height() + ui->Bottom_Bar->height() + ui->widget_borderBottom->height());
}

void GifCreator::setMsBetweenEachScreen(int fps){
    msBeforeNewScreen =(1000/fps);
    ui->spinBox_Fps->setValue(fps);
}


GifCreator::CornerRectSelected GifCreator::getCornerSelected() const{
    return m_cornerSelected;
}

void GifCreator::resizeEvent(QResizeEvent *resizeEvent){
    // We resize all elements and put them in the right place
    /** BOTTOM **
    */
    ui->widget_borderBottom->setGeometry(0, resizeEvent->size().height()-ui->widget_borderBottom->height(), resizeEvent->size().width(), ui->widget_borderBottom->height());
    ui->Bottom_Bar->setGeometry(ui->widget_borderLeft->width(), resizeEvent->size().height() - ui->widget_borderBottom->height() - ui->Bottom_Bar->height(), resizeEvent->size().width() - ui->widget_borderLeft->width() - ui->widget_borderRight->width(), ui->Bottom_Bar->height());
    /**
     *******************/

    /** TOP **
    */
    ui->widget_Top->setGeometry(0, 0, resizeEvent->size().width(), ui->widget_Top->height());
    ui->pushButton_Exit->setGeometry(resizeEvent->size().width() - ui->pushButton_Exit->width() - 10, 0, ui->pushButton_Exit->width(), ui->pushButton_Exit->height());
    /**
    *******************/

    /** MIDDLE **
    */
    // Left
    ui->widget_borderLeft->setGeometry(0, 0, ui->widget_borderLeft->width(), resizeEvent->size().height());

    // Midle
    ui->widget_Photo->setGeometry(ui->widget_borderLeft->width(), ui->widget_Top->size().height(), resizeEvent->size().width() - ui->widget_borderLeft->width() - ui->widget_borderRight->width(), resizeEvent->size().height() - ui->Bottom_Bar->height() - ui->widget_borderBottom->size().height()-21);

    QSvgRenderer renderer(QString(":/GifCreator/rect_photo"));
    QPixmap pix(renderer.defaultSize());

    // Get QPainter that paints to the image
    QPainter painter(&pix);
    renderer.render(&painter);
    //    ui->label_PhotoElements->setGeometry(0, 0,  resizeEvent->size().width() - ui->widget_borderLeft->width() - ui->widget_borderRight->width() - 18, resizeEvent->size().height() - ui->widget_Top->height() - ui->widget_borderBottom->height() - 18);
    //    QPixmap p(":/GifCreator/rect_photo");
    //    ui->label_PhotoElements->setPixmap(p.scaled(QSize(resizeEvent->size().width() - ui->widget_borderLeft->width() - ui->widget_borderRight->width() - 18, resizeEvent->size().height() - ui->widget_Top->height() - ui->widget_borderBottom->height() - 18), Qt::KeepAspectRatio));

    // Right
    ui->widget_borderRight->setGeometry(resizeEvent->size().width() - ui->widget_borderRight->width(), 0, ui->widget_borderRight->width(), resizeEvent->size().height());
    /**
    *******************/


    // Set the spinBox informations
    ui->lineEdit_Width->setText(QString::number(ui->widget_Photo->width()));
    ui->lineEdit_Height->setText(QString::number(ui->widget_Photo->height()));

}

bool GifCreator::isMouseOnWindowCorner(QMouseEvent *event){
    if(ui->widget_Top->geometry().contains(event->pos())){
        if(event->pos().x() <= ui->widget_Top->geometry().x() + 5  && event->pos().x() >= ui->widget_Top->geometry().x()){
            m_cornerSelected = GifCreator::top_left;
            setCursor(Qt::SizeFDiagCursor);
            return true;
        }else if(event->pos().x() >= ui->widget_Top->geometry().right() - 5 && event->pos().x() >= ui->widget_Photo->geometry().right() + 1){
            m_cornerSelected = GifCreator::top_right;
            setCursor(Qt::SizeBDiagCursor);
            return true;
        }else if(event->pos().y() >= ui->widget_Top->geometry().y() -3 && event->pos().y() < ui->widget_Top->geometry().y() + 3){
            m_cornerSelected = GifCreator::top_middle;
            setCursor(Qt::SizeVerCursor);
            return true;
        }
    }

    else if(ui->widget_borderLeft->geometry().contains(event->pos())){
        if(event->pos().y() <= ui->widget_borderLeft->geometry().bottom() - 5){
            m_cornerSelected = GifCreator::middle_left;
            setCursor(Qt::SizeHorCursor);
            return true;
        }

    }else if(ui->widget_borderRight->geometry().contains(event->pos())){
        m_cornerSelected = GifCreator::middle_right;
        setCursor(Qt::SizeHorCursor);
        return true;
    }

    if(ui->widget_borderBottom->geometry().contains(event->pos())){
        if(event->pos().x() <= ui->widget_borderBottom->geometry().x() + 5  && event->pos().x() >= ui->widget_borderBottom->geometry().x() + 5){
            m_cornerSelected = GifCreator::bottom_left;
            setCursor(Qt::SizeBDiagCursor);
            return true;
        }else if(event->pos().x() >= ui->widget_borderBottom->geometry().width() -5 && event->pos().x() < ui->widget_borderBottom->geometry().width() + 5){
            m_cornerSelected = GifCreator::bottom_right;
            setCursor(Qt::SizeFDiagCursor);
            return true;
        }else if(event->pos().y() >= ui->widget_borderBottom->geometry().y() -5 && event->pos().y() < ui->widget_borderBottom->geometry().y() + 5){
            m_cornerSelected = GifCreator::bottom_middle;
            setCursor(Qt::SizeVerCursor);
            return true;
        }
    }

    return false;
}

void GifCreator::callGifEditor(){
    ui->widget_Photo->hide();
    QDesktopServices::openUrl(QUrl(QString("file:///") + QCoreApplication::applicationDirPath() + QString("/") + pathToPictures + QString("/result.gif")));
}

GifCreator::~GifCreator()
{
    delete ui;
}
