#ifndef SCREENSHOTMENU_H
#define SCREENSHOTMENU_H

#include <QWidget>

namespace Ui {
class ScreenShotMenu;
}

class ScreenShotMenu : public QWidget
{
    Q_OBJECT

signals:
    void closeEvent();
    void editEvent();
    void uploadEvent();
    void copyEvent();
    void gifCreatorEvent();
    void widOnlyEvent();

public:
    explicit ScreenShotMenu(QWidget *parent = 0);
    ~ScreenShotMenu();


public slots:

private:
    Ui::ScreenShotMenu *ui;
};

#endif // SCREENSHOTMENU_H
