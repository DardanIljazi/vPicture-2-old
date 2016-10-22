#include "screenshot.h"

/*
 * Worst thing that I've never made in my life
 */
QMap<int, QMap<WId, QRect> > ScreenShot::m_widgetRect;
bool ScreenShot::enumChilds;
/**********************************************/


ScreenShot::ScreenShot(QWidget *parent) :
    QWidget(parent)
{
#ifdef Q_OS_WIN
    refreshWindowsPos();
#endif
}

void ScreenShot::screenshot(){
    selector = new RegionSelect(0, &m_widgetRect);

    int result = selector->exec();

    if (result == QDialog::Accepted)
    {
        m_lastScreenshot = selector->getSelection();
        m_allScreenshot << m_lastScreenshot;

        if(selector->getReturnCode() == RegionSelect::Upload){
            emit screenshotTaken();
        }else if(selector->getReturnCode() == RegionSelect::Copy){
            emit copyEvent();
        }else if(selector->getReturnCode() == RegionSelect::Gif){
            qDebug() << "::GIF" << endl;
            selectRect = selector->lastSelectRect();
            emit gifCreatorEvent();
        }
    }

    selector->deleteLater();
}

QRect ScreenShot::lastSelectRect(){
    return selectRect;
}

QPixmap ScreenShot::getLastScreenshot(){
    return m_lastScreenshot;
}

void ScreenShot::refreshWindowsPos(){
    enumChilds = true;
    m_widgetRect.clear();
#ifdef Q_OS_WIN
    EnumWindows(EnumWindowsProc, 0);
#endif

    if(m_widgetRect.values().count() > 50)
    {
        m_widgetRect.clear();
        qDebug() << "> 50 " << endl;
        enumChilds = false;
#ifdef Q_OS_WIN
        EnumWindows(EnumWindowsProc, 0);
#endif
    }
}

/*
 * Worst code part
 */
#ifdef Q_OS_WIN
BOOL CALLBACK ScreenShot::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    RECT lpRect;
    GetWindowRect(hWnd, &lpRect);
    if(enumChilds)
        EnumChildWindows(hWnd, EnumWindowsProc, 0);

    wchar_t class_name[256], title[256];
    GetClassName(hWnd, class_name, sizeof(class_name));
    GetWindowText(hWnd,title,sizeof(title));

    if(IsWindowVisible(hWnd))
    {
        if(!IsIconic(hWnd))
        {
            //            if((int)lpRect.right - (int)lpRect.left > 0 && (int)lpRect.bottom - (int)lpRect.top > 0 && (int)lpRect.left >= 0 && (int)lpRect.top >= 0)
            //            {
            if( !((lpRect.right - lpRect.left) == QApplication::desktop()->width() &&  (lpRect.bottom - lpRect.top) == QApplication::desktop()->height()) ){
                //                if((lpRect.left >= 0 && lpRect.right <= QApplication::desktop()->width()) && (lpRect.top >= 0 && lpRect.top <= QApplication::desktop()->height())){
                QMap<WId, QRect> map;
                map.insert((WId)hWnd, QRect(lpRect.left, lpRect.top, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top));
                m_widgetRect.insert(m_widgetRect.count(), map);
                //                }
            }
            //            }
        }
    }
    return TRUE;
}

HWND ScreenShot::FindMyTopMostWindow()
{
    DWORD dwProcID = GetCurrentProcessId();
    HWND hWnd = GetTopWindow(GetDesktopWindow());
    while(hWnd)
    {
        DWORD dwWndProcID = 0;
        GetWindowThreadProcessId(hWnd, &dwWndProcID);
        if(dwWndProcID == dwProcID)
            return hWnd;
        hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
    }
    return NULL;
}
#endif
