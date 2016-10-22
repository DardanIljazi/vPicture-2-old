#ifndef MENUMAIN_H
#define MENUMAIN_H

#include <QWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QPropertyAnimation>
#include <QCloseEvent>
#include <QTimer>
#include <QDebug>

namespace Ui {
class MenuMain;
}

class MenuMain : public QWidget
{
    Q_OBJECT

public:
    explicit MenuMain(QWidget *parent = 0);
    ~MenuMain();

    enum Arrow_Side{
        Left_Side,
        Right_Side
    };

public slots:
    void addItems(QList<QListWidgetItem> *items);
    void addItem(QListWidgetItem *item);

    void popup(QPoint point);
    void popup(int x, int y);
    QListWidget* listWidget();
    void setArrowSide(int side = Left_Side);

protected:
    void leaveEvent(QEvent *);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MenuMain *ui;
};

#endif // MENUMAIN_H
