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
#include <QMap>
#include <QHash>
#include <QQueue>
#include <QPair>

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
     * Worst thing that I've never made in my life
     */
    static QMap<int, QMap<WId, QRect> > m_widgetRect;
    static  bool enumChilds;

#ifdef Q_OS_WIN
    static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
    static HWND FindMyTopMostWindow();
#endif
    /***************************************************************/

signals:
    void screenshotTaken(); // Equal to uploadEvent
    void closeEvent();
    void editEvent();
    void uploadEvent();
    void copyEvent();
    void gifCreatorEvent();

public slots:
    /*
     * Called when the user want to take a screenshot
     */
    void screenshot();
    /*
     * Called when widgets position and rects must be refreshed (by EnumWindows)
     */
    void refreshWindowsPos();
    /*
     * Return the last QPixmap from m_lastScreenshot var
     */
    QPixmap getLastScreenshot();

    /*
     * Return the lastSelectRect that come from RegionSelect class to send it to MainWinndo class in the MainWindow::gifCreator() function
     */
    QRect lastSelectRect();

private:
    QPixmap         m_lastScreenshot;
    QList<QPixmap>  m_allScreenshot;

    RegionSelect    *selector;

    // Needed for GifCreator
    QRect selectRect;

protected:

};



#endif // SCREENSHOT_H
