#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QEvent>
#include <QPropertyAnimation>
#include <QTimer>
#include <QMouseEvent>
#include <QDialogButtonBox>
#include <QEventLoop>

#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0, QString title = QString(), QString text = QString());
    ~Dialog();

public slots:
    int box(QWidget *parent = 0, QString title = QString(), QString text = QString(), bool blockUntil=false);
    QDialogButtonBox *buttons();
    int returnButtonValue();
    void setReturnValue(int value);

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

private:
    Ui::Dialog *ui;

    QPoint          m_lastPoint;
    bool  pressed; // Used to know if the mouse has been yet pressed to leave the window
    QPoint lastPos; // When user reduce the  window, the MainWindow::minimizeWindow() function is called. In it, we have to remember the position of the window before the animation is done.
    int returnValue;
};

#endif // DIALOG_H
