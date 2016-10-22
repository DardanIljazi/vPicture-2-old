#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include <QScreen>
#include <QPixmap>
#include <QList>
#include <QGuiApplication>
#include <QtGlobal>
#include <QApplication>
#include <QDesktopWidget>

#include "regionselect.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif



class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    explicit ScreenShot(QWidget *parent = 0);
    /*
     * Certainly the worst thing that I've never made in my life
     */
    static QMap<WId, QRect> m_widgetRect;
#ifdef Q_OS_WIN
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
    static HWND FindMyTopMostWindow();
#endif
    /***************************************************************/

signals:

public slots:
    /*
     * Called when the user want to take a screenshot
     */
    void screenshot();
private:
    QPixmap         m_lastScreenshot;
    QList<QPixmap>  m_allScreenshot;

    RegionSelect    *selector;

protected:

};



#endif // SCREENSHOT_H
