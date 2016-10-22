#ifndef UPDATES_H
#define UPDATES_H

#include <QWidget>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QMouseEvent>
#include <QDialogButtonBox>
#include <QEventLoop>
#include <QProcess>
#include "../Errors/errors.h"
#include <QDebug>

namespace Ui {
class Updates;
}

class Updates : public QWidget
{
    Q_OBJECT

public:
    explicit Updates(QWidget *parent = 0);
    ~Updates();

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
    void setProgression(qint64 bytesReceived, qint64 bytesTotal);
    void saveAndLaunchSetup(QByteArray dataSetupLauncher);
    void launchSoft(int exitCode=-10000, QProcess::ExitStatus exitStatus=QProcess::NormalExit);

private:
    Ui::Updates *ui;

    Errors *localErrors;

    QPoint          m_lastPoint;
    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.

    QProcess *launcherProcess;


};

#endif // UPDATES_H
