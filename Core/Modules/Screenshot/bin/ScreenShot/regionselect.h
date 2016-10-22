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
#include <QDebug>


class RegionSelect : public QDialog
{
public:
    RegionSelect(QWidget *parent = 0, QMap<WId, QRect> *rects = 0);
    ~RegionSelect();
    QPixmap getSelection();

protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QRect selectRect;
    QSize sizeDesktop;

    QPoint selStartPoint;
    QPoint selEndPoint;

    bool palBackground;
    bool pressed;

    QScreen *screen;

    QPixmap desktopPixmapBkg;
    QPixmap desktopPixmapClr;

    QMap<WId, QRect> *m_widgetRect;

    QPoint posMouse;

    void drawBackGround();
    void drawRectSelection(QPainter &painter);
    void drawLines(QPoint pos, QPainter &painter);

};

#endif // REGIONSELECT_H
