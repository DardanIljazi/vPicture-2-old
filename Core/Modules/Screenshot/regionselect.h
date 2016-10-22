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

#ifndef REGIONSELECT_H
#define REGIONSELECT_H

#include <QDialog>

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QSize>
#include <QPoint>
#include <QGuiApplication>
#include <QScreen>
#include <QToolButton>
#include <QTimer>
#include <QHash>
#include <QQueue>
#include <QPair>
#include <QPropertyAnimation>

#include "screenshotmenu.h"
#include "../GifCreator/gifcreator.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif


class RegionSelect : public QDialog
{
    Q_OBJECT
    Q_ENUMS(CornerRectSelected)
public:
    RegionSelect(QWidget *parent = 0, QMap<int, QMap<WId, QRect> >  *rects = 0);
    ~RegionSelect();
    QPixmap getSelection();
    enum CornerRectSelected {top_left, top_right, middle_left, bottom_left, bottom_right, middle_right, middle_top, middle_bottom};
    CornerRectSelected getCornerSelected() const;
    enum ReturnCode{
        Close,
        Edit,
        Upload,
        Copy,
        Gif
    };

signals:
    void closeEvent();
    void editEvent();
    void uploadEvent();
    void copyEvent();
    void gifCreatorEvent();

protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);
    //    void mouseMoveEvent(QMouseEvent *event);

private:
    QRect selectRect;
    QList<QRect> cornersSelection;
    bool pressedOnCornerRect;
    CornerRectSelected m_cornerSelected;
    QSize sizeDesktop;

    QPoint selStartPoint;
    QPoint selEndPoint;

    bool palBackground;
    bool pressed;
    bool selectedWindow;
    bool animationBegan;
    bool copyPicturePressed;

    QScreen *screen;

    QPixmap desktopPixmapBkg;
    QPixmap desktopPixmapClr;
    QPixmap desktopPixmapBlackBkg;

    QMap<int, QMap<WId, QRect> > *m_widgetRect, lastWidgetHovered;

    QPoint posMouse;

    QPen animatedPen;

    /*
     * UseWid: Client can choose by a button if he wants to take screenshot by Wid or not.
     * Wid option grab only the window selected, ommiting all the others windows even if they are on the window that has been selected
     */
    QToolButton *m_useWidButton;
    bool useWidOnly;

    void drawBackGround();
    void drawRectSelection(QPainter &painter, QPen pen = QPen(QBrush(Qt::red), 5, Qt::DotLine));
    void drawLines(QPoint pos, QPainter &painter);

    ScreenShotMenu *localScreenShotMenu;
    GifCreator *localGifCreator;

    int returnCode, numbLoop;

    QString textToDraw;

public slots:
    void useWidScreenOnly();
    QMap<int, QMap<WId, QRect> > whichWindowHovered(QMouseEvent *event);

    bool isMouseOnCorner(QMouseEvent *event);
    bool isMouseOnSelectRectCorner(QMouseEvent *event);
    void animationDotLineRect(QPainter &painter);
    int underMouseContainsWindow(QPoint point);
    void moveScreenShotMenuToRightPosition();

    QRect lastSelectRect();

    void close();
    void edit();
    void upload();
    void copy();
    void gifCreator();

    int getReturnCode();
    void animationCopyEvent();

};

#endif // REGIONSELECT_H
