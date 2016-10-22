#include "screenshot.h"

/*
 * Worst thing that I've never made in my life
 */
QMap<WId, QRect> ScreenShot::m_widgetRect;
/**********************************************/


ScreenShot::ScreenShot(QWidget *parent) :
    QWidget(parent)
{
    EnumWindows(EnumWindowsProc, 0);
}

void ScreenShot::screenshot(){
    selector = new RegionSelect(0, &m_widgetRect);

    int result = selector->exec();

    if (result == QDialog::Accepted)
    {
        m_lastScreenshot = selector->getSelection();
        m_allScreenshot << m_lastScreenshot;

        m_lastScreenshot.save("test.png", "PNG");
    }

    delete selector;
}

/*
 * Worst code part
 */
#ifdef Q_OS_WIN
BOOL CALLBACK ScreenShot::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    RECT lpRect;
    GetWindowRect(hWnd, &lpRect);
    EnumChildWindows(hWnd, EnumWindowsProc, 0);



    //    wchar_t class_name[256], title[256];
    //    GetClassName(hWnd, class_name, sizeof(class_name));
    //    GetWindowText(hWnd,title,sizeof(title));

    if(IsWindowVisible(hWnd))
    {
        if(!IsIconic(hWnd))
        {
//            if((int)lpRect.right - (int)lpRect.left > 0 && (int)lpRect.bottom - (int)lpRect.top > 0 && (int)lpRect.left >= 0 && (int)lpRect.top >= 0)
//            {
                if( !((lpRect.right - lpRect.left) == QApplication::desktop()->width() &&  (lpRect.bottom - lpRect.top) == QApplication::desktop()->height()) )
                    m_widgetRect.insert((WId)hWnd, QRect(lpRect.left, lpRect.top, lpRect.right - lpRect.left, lpRect.bottom - lpRect.top));
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
