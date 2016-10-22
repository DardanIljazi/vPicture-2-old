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
 * Code adapted by Dardanboy
 */
#include "regionselect.h"

#include <QDesktopWidget>
#include <QApplication>

RegionSelect::RegionSelect(QWidget *parent, QMap<WId, QRect> *rects)
    :QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setWindowState(Qt::WindowFullScreen);
    setCursor(Qt::CrossCursor);

    pressed = false;

    sizeDesktop = QApplication::desktop()->size();
    resize(sizeDesktop);

    screen = QGuiApplication::primaryScreen();
    desktopPixmapBkg = screen->grabWindow(QApplication::desktop()->winId());
    desktopPixmapClr = desktopPixmapBkg;

    m_widgetRect = rects;

    setMouseTracking(true);
    move(0, 0);
    drawBackGround();
}

RegionSelect::~RegionSelect()
{

}

bool RegionSelect::event(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::KeyPress)
    {
        pressed = false;
        accept();
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        pressed = true;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);

        if (mouseEvent->button() != Qt::LeftButton)
            reject();

        selStartPoint = mouseEvent->pos();
        selectRect = QRect(selStartPoint, QSize());

        for(int i = 0; i < m_widgetRect->values().count() ; i ++){
            if(m_widgetRect->values().at(i).contains(mouseEvent->pos())){
                selectRect = QRect(m_widgetRect->values().at(i));
                selEndPoint = m_widgetRect->values().at(i).topLeft();
            }
        }

        if(mouseEvent->pos() == QPoint(0, 0)
                || mouseEvent->pos() == QPoint(QApplication::desktop()->width()-1, 0)
                || mouseEvent->pos() == QPoint(QApplication::desktop()->width()-1, QApplication::desktop()->height()-1)
                || mouseEvent->pos() == QPoint(0, QApplication::desktop()->height()-1)){
            selectRect = QRect(QApplication::desktop()->rect());
            selEndPoint = QApplication::desktop()->pos();
        }


    }

    return QDialog::event(event);
}

void RegionSelect::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (!palBackground)
        painter.drawPixmap(QPoint(0, 0), desktopPixmapBkg);

    drawRectSelection(painter);
    drawLines(posMouse, painter);
}

void RegionSelect::mouseMoveEvent(QMouseEvent *event)
{
    // Verify if a window is present in the mouse zone
    for(int i = 0; i < m_widgetRect->values().count() ; i ++){
        if(m_widgetRect->values().at(i).contains(event->pos())){
            selectRect = QRect(m_widgetRect->values().at(i));
            selEndPoint = m_widgetRect->values().at(i).topLeft();
        }
    }
    // If the user goes to top-left points, it select all the window
    if(event->pos() == QPoint(0, 0)
            || event->pos() == QPoint(QApplication::desktop()->width()-1, 0)
            || event->pos() == QPoint(QApplication::desktop()->width()-1, QApplication::desktop()->height()-1)
            || event->pos() == QPoint(0, QApplication::desktop()->height()-1)){
        selectRect = QRect(QApplication::desktop()->rect());
        selEndPoint = QApplication::desktop()->pos();
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);
    if(pressed){
        selectRect = QRect(selStartPoint, mouseEvent->pos()).normalized();
        selEndPoint  = mouseEvent->pos();
    }

    posMouse = event->pos();

    update();
}

void RegionSelect::drawLines(QPoint pos, QPainter &painter){
    painter.setPen(QPen(QBrush(Qt::gray), 1));
    painter.drawLine(pos.x(), QApplication::desktop()->height(), pos.x(), 0);
    painter.drawLine(QApplication::desktop()->width(), pos.y(), 0, pos.y());
}

void RegionSelect::drawBackGround()
{
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

void RegionSelect::drawRectSelection(QPainter &painter)
{
    painter.drawPixmap(selectRect, desktopPixmapClr, selectRect);
    painter.setPen(QPen(QBrush(Qt::red), 2));

    painter.drawRect(QRect(selectRect.x()+1, selectRect.y()+1, selectRect.width()-2, selectRect.height()-2));

    //    QString txtSize = QApplication::tr("%1 x %2").arg(selectRect.width()).arg(selectRect.height());
    //    painter.setPen(QPen(QBrush(Qt::black), 2));
    //    painter.drawText(QRect(selectRect.x(), selectRect.y(), selectRect.width()-3, selectRect.height()-3), Qt::AlignBottom | Qt::AlignRight, txtSize);
}

QPixmap RegionSelect::getSelection()
{
    QPixmap sel;
    sel = desktopPixmapClr.copy(selectRect);
    return sel;
}
