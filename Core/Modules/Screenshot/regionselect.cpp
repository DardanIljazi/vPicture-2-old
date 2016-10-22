/***************************************************************************
 *   Copyright (C) 2009 - 2011 by Artem 'DOOMer' Galichkin                        *
 *   doomer3d@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*
 * Code adapted by Dardanboy, CogDev Team
 */

#include "regionselect.h"

#include <QDesktopWidget>
#include <QApplication>

RegionSelect::RegionSelect(QWidget *parent, QMap<int, QMap<WId, QRect> > *rects)
    :QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setWindowState(Qt::WindowFullScreen);
    setCursor(Qt::CrossCursor);

    localScreenShotMenu = new ScreenShotMenu(this);
    localScreenShotMenu->hide();

    pressed = false;
    selectedWindow = false;
    pressedOnCornerRect = false;
    animationBegan = false;
    copyPicturePressed = false;
    numbLoop = 0;

    animatedPen = QPen(QBrush(QColor(241, 196, 15, 255)), 3);
    QVector<qreal> dashes;
    dashes << 1 << 3;
    //    animatedPen.setDashPattern(dashes);

    m_useWidButton = new QToolButton(this);
    m_useWidButton->setStyleSheet("background: none;");
    m_useWidButton->setIcon(QIcon(":/Screenshot/UseWidButtonNoPressed"));
    m_useWidButton->setIconSize(QSize(50, 30));
    m_useWidButton->setToolTip(tr("Screen only this window"));
    m_useWidButton->setCursor(Qt::PointingHandCursor);
    m_useWidButton->hide();

    useWidOnly = false;

    sizeDesktop = QApplication::desktop()->size();
    resize(sizeDesktop);

    screen = QGuiApplication::primaryScreen();
    desktopPixmapBkg = screen->grabWindow(QApplication::desktop()->winId());
    desktopPixmapClr = desktopPixmapBkg;
    m_widgetRect = rects;
    qDebug() << m_widgetRect->values() << endl;



    setMouseTracking(true);
    move(0, 0);
    drawBackGround();

    QObject::connect(m_useWidButton, SIGNAL(clicked()), this, SLOT(useWidScreenOnly()));

    /*** REFERED TO localScreenShotMenu Class signals ***/

    QObject::connect(localScreenShotMenu, SIGNAL(closeEvent()), this, SLOT(reject()));

    QObject::connect(localScreenShotMenu, SIGNAL(uploadEvent()), this, SLOT(upload()));


    //    QObject::connect(localScreenShotMenu, SIGNAL(editEvent()), this, SLOT());
    //    QObject::connect(localScreenShotMenu, SIGNAL(copyEvent()), this, SLOT(accept()));
    //    QObject::connect(localScreenShotMenu, SIGNAL(copyEvent()), this, SLOT(copy()));
    QObject::connect(localScreenShotMenu, SIGNAL(copyEvent()), this, SLOT(animationCopyEvent()));
    QObject::connect(localScreenShotMenu, SIGNAL(widOnlyEvent()), this, SLOT(useWidScreenOnly()));

    QObject::connect(localScreenShotMenu, SIGNAL(gifCreatorEvent()), this, SLOT(accept()));
    QObject::connect(localScreenShotMenu, SIGNAL(gifCreatorEvent()), this, SLOT(gifCreator()));

}

void RegionSelect::useWidScreenOnly(){
    if(!useWidOnly){
        m_useWidButton->setIcon(QIcon(":/Screenshot/UseWidButton"));
        useWidOnly = true;
    }else{
        m_useWidButton->setIcon(QIcon(":/Screenshot/UseWidButtonNoPressed"));
        useWidOnly = false;
    }
}

QRect RegionSelect::lastSelectRect(){
    return selectRect;
}

void RegionSelect::close(){
    returnCode = RegionSelect::Close;
    emit closeEvent();
}

void RegionSelect::edit(){
    returnCode = RegionSelect::Edit;
    emit editEvent();
}

void RegionSelect::upload(){
    returnCode = RegionSelect::Upload;
    emit uploadEvent();
    accept();
}

void RegionSelect::copy(){
    numbLoop = 0;
    copyPicturePressed = false;
    returnCode = RegionSelect::Copy;
    emit copyEvent();
    accept();
}

void RegionSelect::gifCreator(){
    returnCode = RegionSelect::Gif;
    emit gifCreatorEvent();
}

int RegionSelect::getReturnCode(){
    return returnCode;
}

RegionSelect::~RegionSelect()
{

}

bool RegionSelect::event(QEvent *event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);

    if (event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*> (event);
        if(keyEvent->key() == Qt::Key_Escape){
            reject();
        }

        pressed = false;
        if(mouseEvent->button() == Qt::LeftButton){
            if(pressedOnCornerRect){
                selEndPoint = mouseEvent->pos();
                pressedOnCornerRect = false;
            }else{
                if(!selectedWindow){
                    selectedWindow = true;
                    if(useWidOnly)
                        accept();
                    else{
                        update();
                    }
                }
                else{
                    if(selectRect.contains(mouseEvent->pos())){
                        this->upload();
                    }
                }
            }
        }
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;

        if (mouseEvent->button() != Qt::LeftButton){
            if(!selectedWindow){
                if(useWidOnly)
                    useWidScreenOnly();
                else
                    reject();
            }

            if(selectedWindow){
                selectedWindow = false;
                cornersSelection.clear();
                update();
            }
        }else{
            if(isMouseOnSelectRectCorner(mouseEvent)){
                pressedOnCornerRect = true;
            }
        }

        selStartPoint = mouseEvent->pos();
        //        selectRect = QRect(selStartPoint, QSize());
    }

    if(event->type() == QEvent::MouseMove){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);

        if(!isMouseOnSelectRectCorner(mouseEvent)){
            setCursor(Qt::CrossCursor);
        }

        if(pressedOnCornerRect){
            if(getCornerSelected() == RegionSelect::top_left){
                selectRect = QRect(selectRect.bottomRight(), mouseEvent->pos()).normalized();
            }else if(getCornerSelected() == RegionSelect::top_right){
                selectRect = QRect(selectRect.bottomLeft(), mouseEvent->pos()).normalized();
            }else if(getCornerSelected() == RegionSelect::bottom_right){
                selectRect = QRect(selectRect.topLeft(), mouseEvent->pos()).normalized();
            }else if(getCornerSelected() == RegionSelect::bottom_left){
                selectRect = QRect(selectRect.topRight(), mouseEvent->pos()).normalized();
            }else if(getCornerSelected() == RegionSelect::middle_left){
                selectRect = QRect(selectRect.bottomRight(), QPoint(selectRect.x() +(mouseEvent->x() - selectRect.x()), selectRect.y())).normalized();
            }else if(getCornerSelected() == RegionSelect::middle_right){
                selectRect = QRect(selectRect.bottomLeft(), QPoint(selectRect.x() +(mouseEvent->x() - selectRect.x()), selectRect.y())).normalized();
            }else if(getCornerSelected() == RegionSelect::middle_top){
                selectRect = QRect(selectRect.bottomRight(), QPoint(selectRect.x(), selectRect.y() +(mouseEvent->y() - selectRect.y()))).normalized();
            }else if(getCornerSelected() == RegionSelect::middle_bottom){
                selectRect = QRect(selectRect.topRight(), QPoint(selectRect.x(), selectRect.y() +(mouseEvent->y() - selectRect.y()))).normalized();
            }
            update();
        }else{
            if(!pressed)
                lastWidgetHovered = whichWindowHovered(mouseEvent);

            if(isMouseOnCorner(mouseEvent)){
                selectRect = QRect(QApplication::desktop()->rect());
                qDebug() << "selectRect desktop: " << selectRect << endl;
                selEndPoint = QApplication::desktop()->pos();
            }

            if(pressed){
                selectedWindow = false;
                selectRect = QRect(selStartPoint, mouseEvent->pos()).normalized();
                selEndPoint  = mouseEvent->pos();
            }
        }

        posMouse = mouseEvent->pos();

        update();
    }

    return QDialog::event(event);
}

RegionSelect::CornerRectSelected RegionSelect::getCornerSelected() const{
    return m_cornerSelected;
}

bool RegionSelect::isMouseOnSelectRectCorner(QMouseEvent *event){
    for(int i = 0; i < cornersSelection.count(); i ++){
        if(cornersSelection.at(i).contains(event->pos())){
            if(i == RegionSelect::top_left){
                m_cornerSelected = RegionSelect::top_left;
                setCursor(Qt::SizeFDiagCursor);
            }else if(i == RegionSelect::top_right){
                m_cornerSelected = RegionSelect::top_right;
                setCursor(Qt::SizeBDiagCursor);
            }else if(i == RegionSelect::middle_left){
                m_cornerSelected = RegionSelect::middle_left;
                setCursor(Qt::SizeHorCursor);
            }else if(i ==  RegionSelect::bottom_left){
                m_cornerSelected = RegionSelect::bottom_left;
                setCursor(Qt::SizeBDiagCursor);
            }else if(i == RegionSelect::bottom_right){
                m_cornerSelected = RegionSelect::bottom_right;
                setCursor(Qt::SizeFDiagCursor);
            }else if(i == RegionSelect::middle_right){
                m_cornerSelected = RegionSelect::middle_right;
                setCursor(Qt::SizeHorCursor);
            }else if(i == RegionSelect::middle_top){
                m_cornerSelected = RegionSelect::middle_top;
                setCursor(Qt::SizeVerCursor);
            }else if(i == RegionSelect::middle_bottom){
                m_cornerSelected = RegionSelect::middle_bottom;
                setCursor(Qt::SizeVerCursor);
            }
            return true;
        }
    }
    return false;
}

void RegionSelect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (!palBackground)
        painter.drawPixmap(QPoint(0, 0), desktopPixmapBkg);

    if(!copyPicturePressed){
        drawRectSelection(painter);
        drawLines(posMouse, painter);
    }else{
        animatedPen = QPen(QBrush(QColor(236, 240, 241, 255)), 7);
        drawRectSelection(painter, animatedPen);
    }




    if(copyPicturePressed){
        animatedPen = QPen(QBrush(QColor(236, 240, 241, numbLoop*17)), 7);
        animatedPen.setJoinStyle(Qt::MiterJoin);
        if(numbLoop < 16){

            drawRectSelection(painter, animatedPen);
            numbLoop ++;
            QTimer::singleShot(26, this, SLOT(update()));
        }else{
            painter.setPen(QPen(QBrush(QColor(39, 174, 96, 175)), 20));
            if(selectRect.height() < selectRect.width())
                painter.setFont(QFont("Open Sans", ((selectRect.height())/1.618033988751)/4, 4));
            else
                painter.setFont(QFont("Open Sans", ((selectRect.width())/1.61803398875)/4, 4));

            painter.drawText(selectRect, Qt::AlignCenter, tr("Copied!"));
            QTimer::singleShot(750, this, SLOT(copy()));
        }
    }else{
        if(selectedWindow){
            if(selectRect.width() > 18 && selectRect.height() > 18){
                localScreenShotMenu->show();
                moveScreenShotMenuToRightPosition();

                drawRectSelection(painter, animatedPen);

                cornersSelection.clear();
                // We create the circles that are on the coins and in the middle
                cornersSelection << QRect(selectRect.topLeft() - QPoint(1, 1), QSize(8, 8));                                // top_left
                cornersSelection << QRect(selectRect.topRight() - QPoint(5, 1), QSize(8, 8));                               // top_right
                cornersSelection << QRect(selectRect.topLeft() + QPoint(-1, selectRect.height()/2 - 3) , QSize(8, 8));      // middle_left
                cornersSelection << QRect(selectRect.topLeft() + QPoint(-1, selectRect.height() -6), QSize(8, 8));          // bottom_left
                cornersSelection << QRect(selectRect.topRight() + QPoint(-5, selectRect.height() - 6), QSize(8, 8));        // bottom_right
                cornersSelection << QRect(selectRect.topRight() + QPoint(-5, selectRect.height()/2 - 3), QSize(8, 8));      // middle_right
                cornersSelection << QRect(selectRect.topLeft() + QPoint((selectRect.width())/2 - 3, -1), QSize(8, 8));      // middle_top
                cornersSelection << QRect(selectRect.bottomLeft() + QPoint(selectRect.width()/2 - 3, -5), QSize(8, 8));     // middle_bottom

                for(int i = 0; i < 8; i++){
                    painter.fillRect(cornersSelection.at(i), QBrush(QColor(241, 196, 15, 255)));
                }
                QPen pen = painter.pen();
                pen.setJoinStyle(Qt::MiterJoin);
                painter.setPen(pen);
                //        painter.setPen(QPen(QBrush(QColor(243, 156, 18, 255)), 2));
                //        painter.drawRects(cornersSelection.toVector());
                painter.drawRects(cornersSelection.toVector());
            }else{
                if(!lastWidgetHovered.isEmpty()){
                    selectRect = QRect(lastWidgetHovered.values().first().first());
                    selEndPoint = lastWidgetHovered.values().first().first().topLeft();
                }
                update();
            }
        }else{
            localScreenShotMenu->hide();
        }
    }

    if(useWidOnly){
        painter.fillRect(QApplication::desktop()->rect(), Qt::black);
        //        painter.drawPixmap(selectRect, screen->grabWindow(lastWidgetHovered));
        if(!lastWidgetHovered.isEmpty())
            painter.drawPixmap(selectRect.x(), selectRect.y(), screen->grabWindow(lastWidgetHovered.first().firstKey()).width(), screen->grabWindow(lastWidgetHovered.first().firstKey()).height(), screen->grabWindow(lastWidgetHovered.first().firstKey()));

    }
}

void RegionSelect::moveScreenShotMenuToRightPosition(){
    localScreenShotMenu->move(selectRect.x() + selectRect.width()+1, selectRect.y());

    //    qDebug() << "localScreenShotMenu->pos().x() = " << localScreenShotMenu->pos().x() << " pos().y() " << localScreenShotMenu->pos().y() << endl;
    // If the localScreenShotMenu go out of the screen on the bottom side, the localScreenShotMenu is moved in the top of selectRect
    if(localScreenShotMenu->pos().y() + localScreenShotMenu->height() > QApplication::desktop()->height()){
        localScreenShotMenu->move(selectRect.x() + selectRect.width()+1 ,selectRect.y() - localScreenShotMenu->height());
    }
    // If the localScreenShotMenu go out of the screen on the right side, the localScreenShotMenu is moved to the left side
    if(localScreenShotMenu->pos().x() + localScreenShotMenu->width() > QApplication::desktop()->width()){
        localScreenShotMenu->move(selectRect.x() - localScreenShotMenu->width(), selectRect.y());
    }
    // If the localScreenShotMenu go out of the screen on the left side, the localScreenShotMenu is moved in the selectRect on the right side
    if(localScreenShotMenu->pos().x() < 0){
        localScreenShotMenu->move(selectRect.x() + selectRect.width() - localScreenShotMenu->width(), selectRect.y());
    }
    if(localScreenShotMenu->pos().y() + localScreenShotMenu->height() > QApplication::desktop()->height()){
        localScreenShotMenu->move(selectRect.x() + selectRect.width() - localScreenShotMenu->width() ,selectRect.y() - localScreenShotMenu->height());
    }
}

/*
 * Function that returns the QMap with Wid and QRect for the window hovered by the mouse
 */
QMap<int, QMap<WId, QRect> > RegionSelect::whichWindowHovered(QMouseEvent *event){
    QMap<int, QMap<WId, QRect> > lastWidget;
    lastWidget.clear();
    int nbWindowsFound = 0;
    //#ifdef Q_OS_WIN
    //    POINT P;
    //    HWND Handle = NULL;
    //    GetCursorPos(&P);

    //    RECT rect;
    //    GetWindowRect(WindowFromPoint(P),&rect);
    //    ChildWindowFromPoint((HWND)this->winId(), P);
    //#endif

    // Verify if a window is present in the mouse zone
    for(int i = 0; i < m_widgetRect->values().count() ; i ++)
    {
        if(m_widgetRect->values().at(i).first().contains(event->pos()))
        {
            nbWindowsFound ++;
        }
    }

    for(int i = 0; i < m_widgetRect->values().count() ; i ++)
    {
        if(m_widgetRect->values().at(i).first().contains(event->pos()))
        {
            lastWidget.insert(m_widgetRect->keys().at(i), m_widgetRect->values().at(i));
            if(!selectedWindow){
                selectRect = QRect(m_widgetRect->values().at(i).first());
                selEndPoint = m_widgetRect->values().at(i).first().topLeft();
            }
            if(nbWindowsFound > 1){
                i = m_widgetRect->values().count();
            }
        }
    }


    return lastWidget;
}

int RegionSelect::underMouseContainsWindow(QPoint point){
    // Verify if a window is present in the mouse zone
    for(int i = 0; i < m_widgetRect->values().count(); i ++){
        if(m_widgetRect->values().at(i).first().contains(point, true)){
            return i;
        }
    }

    return -1;
}

/*
 * Function that returns true if the mouse is on one corner of the screen
 * This function is used for example to select all the screen zone when the mouse is on one corner (top-left|right, bottom-left|right)
 */
bool RegionSelect::isMouseOnCorner(QMouseEvent *event){
    // If the user goes to top-left, top-right, bottom-left, bottom-right points, it select all the window
    if(event->pos() == QPoint(0, 0)
            || event->pos() == QPoint(QApplication::desktop()->width()-1, 0) // ->width()-1 is a cheat because the mouse can't go to the maximum size of the screen
            || event->pos() == QPoint(QApplication::desktop()->width()-1, QApplication::desktop()->height()-1)
            || event->pos() == QPoint(0, QApplication::desktop()->height()-1)){
        return true;
    }

    return false;
}

void RegionSelect::drawLines(QPoint pos, QPainter &painter){
    painter.setPen(QPen(QBrush(QColor(149, 165, 166, 255)), 1));
    painter.drawLine(pos.x(), QApplication::desktop()->height(), pos.x(), 0);
    painter.drawLine(QApplication::desktop()->width(), pos.y(), 0, pos.y());
}

void RegionSelect::drawBackGround()
{
    desktopPixmapBlackBkg.fill(Qt::black);
    // create painter on  pixelmap of desktop
    QPainter painter(&desktopPixmapBkg);

    // set painter brush on 85% transparency
    painter.setBrush(QBrush(QColor(0, 0, 0, 100), Qt::SolidPattern));

    // draw rect of desktop size in poainter
    painter.drawRect(QApplication::desktop()->rect());

    //    QRect txtRect = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());
    //    QString txtTip = QApplication::tr("Use your mouse to draw a rectangle to screenshot or exit pressing\nany key or using the right or middle mouse buttons.");

    //    txtRect.setHeight(qRound((double)txtRect.height() / 10)); // rounded val of text rect height

    painter.setPen(QPen(Qt::red)); // ste message rect border color
    painter.setBrush(QBrush(QColor(255, 255, 255, 180), Qt::SolidPattern));
    //    QRect txtBgRect = painter.boundingRect(txtRect, Qt::AlignCenter, txtTip);

    //    // set height & width of bkg rect
    //    txtBgRect.setX(txtBgRect.x() - 6);
    //    txtBgRect.setY(txtBgRect.y() - 4);
    //    txtBgRect.setWidth(txtBgRect.width() + 12);
    //    txtBgRect.setHeight(txtBgRect.height() + 8);

    //    painter.drawRect(txtBgRect);

    // Draw the text
    //    painter.setPen(QPen(Qt::black)); // black color pen
    //    painter.drawText(txtBgRect, Qt::AlignCenter, txtTip);

    palBackground = (qApp->desktop()->numScreens() > 1);

    // set bkg to pallette widget
    if (palBackground)
    {
        QPalette newPalette = palette();
        newPalette.setBrush(QPalette::Window, QBrush(desktopPixmapBkg));
        setPalette(newPalette);
    }
}

void RegionSelect::drawRectSelection(QPainter &painter, QPen pen)
{
    painter.drawPixmap(selectRect, desktopPixmapClr, selectRect);
    painter.setPen(pen);

    painter.drawRect(QRect(selectRect.x()+1, selectRect.y()+1, selectRect.width()-2, selectRect.height()-2));

    if(selectedWindow){
        if(!animationBegan)
            animationDotLineRect(painter);
    }

    //    QString txtSize = QApplication::tr("%1 x %2").arg(selectRect.width()).arg(selectRect.height());
    //    painter.setPen(QPen(QBrush(Qt::black), 2));
    //    painter.drawText(QRect(selectRect.x(), selectRect.y(), selectRect.width()-3, selectRect.height()-3), Qt::AlignBottom | Qt::AlignRight, txtSize);
}

void RegionSelect::animationDotLineRect(QPainter &painter){
    /*    static QPainter &painter2(painter);
    animationBegan = true;
    animatedPen.setDashOffset(animatedPen.dashOffset() + 0.1);
    painter2.setPen(animatedPen);

    painter2.drawRect(QRect(selectRect.x()+1, selectRect.y()+1, selectRect.width()-2, selectRect.height()-2));

//    QTimer::singleShot(750, this, SLOT(animationDotLineRect(QPainter&)))*/;
}

QPixmap RegionSelect::getSelection()
{
    QPixmap sel;
    if(useWidOnly)
        sel = screen->grabWindow(lastWidgetHovered.values().first().firstKey());
    else
        sel = desktopPixmapClr.copy(selectRect);

    return sel;
}

void RegionSelect::animationCopyEvent(){
    copyPicturePressed = true;

    repaint();
}
