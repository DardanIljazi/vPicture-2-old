#ifndef GIFCREATOR_H
#define GIFCREATOR_H

#include <QWidget>
#include <QScreen>
#include <QToolButton>
#include <QDesktopWidget>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QDesktopServices>

#include "../../Dialog/dialog.h"
#include "../../Config/settings.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

namespace Ui {
class GifCreator;
}

class GifCreator : public QWidget
{
    Q_OBJECT

public:
    explicit GifCreator(QWidget *parent = 0, Settings *settings = 0);
    ~GifCreator();

    enum CornerRectSelected {top_left, top_middle, top_right, middle_left, bottom_left, bottom_middle, bottom_right, middle_right};
    CornerRectSelected getCornerSelected() const;

public slots:
    /*
     * Function called to start the process of taking screenshot
     */
    void start();

    /*
     * Function called to take each photo
     */
    void process();

    /*
     * Function called each x ms to put in the listPngToSave variable new QPixmap no save later in the save() function
     */
     void saveToList();
     /*
      * Function called before save to set the thread
      */
     void threadSave();
    /*
     * Function called to save pictures that are in the listPngToSave list
     */
    void save();

    /*
     * Function that stop the screen actions
     */
    void stop();

    /*
     * Function called directly after the stop() function
     */
    void convertToGif();

    /*
     * Function that is called directly after the stop() function
     */
    void saveGif();

    /*
     * Function called when the action is paused
     */
    void pause();

    /*
     * Function called after pause() function is called to unblock the pause
     */
    void deblockPause();

    /*
     * Function called to change the region to save
     */
    void setRect(QRect rect);

    /*
     * Function called to change the number of ms between each screen
     */
    void setMsBetweenEachScreen(int fps);

    /*
     * Function called every times the mouse leave to detect if the mouse is on the corner
     */
    bool isMouseOnWindowCorner(QMouseEvent *event);

    /*
     * Function called when the spinbox for width is changed
     */
    void resizeWidth(int width);

    /*
     * Function called when the spinbox for height is changed
     */
    void resizeHeight(int height);

    /*
     * Function called after convertToGif() for waiting the gif animated finished
     */
    void waitGifAnimatedFinished();

    /*
     * Function called every second after waitGifAnimatedFinished() is called
     */
    bool isGifAnimatedFinished();

    /*
     * Function called to call the Gif Editor after all the actions (saving, making gif are done)
     */
    void callGifEditor();

    /*
     * ImageMagick convert errors
     */
    void showImageMagickConvertErrors();


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

    /*
     * Function called when the window is resized
     */
    void resizeEvent(QResizeEvent *resizeEvent);


private:
    Ui::GifCreator *ui;
    int msBeforeNewScreen, numbPixmap;
    bool stopAction;
    QRect lastRect;
    QScreen *screen;
    QPixmap desktopScreen;
    QPixmap rectScreen;
    QString tempName;
    QDir dir;
    QString pathToPictures;
    QList<QPixmap> listPngToSave;

    QProcess *convertImageMagick;

    QPoint          m_lastPoint;
    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.

    CornerRectSelected m_cornerSelected;
    bool pressedOnCorner;

    Settings *localSettings;
};

#endif // GIFCREATOR_H
