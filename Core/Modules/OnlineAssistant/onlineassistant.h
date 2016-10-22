#ifndef ONLINEASSISTANT_H
#define ONLINEASSISTANT_H

#include <QWidget>
#include <QDesktopServices>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>

namespace Ui {
class OnlineAssistant;
}

class OnlineAssistant : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineAssistant(QWidget *parent = 0);
    ~OnlineAssistant();

protected:
    /*
     * Function called when a leaveEvent
     */
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    /*
     * Function called when the user exit the app
     */
    void closeEvent(QCloseEvent *event);

    void showEvent(QShowEvent *event);

public slots:
    void openPremiumUrl();
    void openSecurityUrl();
    void openOnlineHelpWidget();
    void openUploadUrl();
    void openScreenshotUrl();
    void openGifCreatorUrl();
    void openImageEditorUrl();

    void detectHowManyChar();

    void changeStackedWidgetToMain();

private:
    Ui::OnlineAssistant *ui;

    QPoint          m_lastPoint;
    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.
};

#endif // ONLINEASSISTANT_H
