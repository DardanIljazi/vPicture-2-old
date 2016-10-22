#include "screenshotmenu.h"
#include "ui_screenshotmenu.h"

ScreenShotMenu::ScreenShotMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShotMenu)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton_Close, SIGNAL(pressed()), this, SIGNAL(closeEvent()));
//    QObject::connect(ui->pushButton_Edit, SIGNAL(pressed()), this, SIGNAL(editEvent()));
    QObject::connect(ui->pushButton_Upload, SIGNAL(pressed()), this, SIGNAL(uploadEvent()));
    QObject::connect(ui->pushButton_Copy, SIGNAL(pressed()), this, SIGNAL(copyEvent()));
    QObject::connect(ui->pushButton_GifCreator, SIGNAL(pressed()), this, SIGNAL(gifCreatorEvent()));
//    QObject::connect(ui->pushButton_WIdOnly, SIGNAL(pressed()), this, SIGNAL(widOnlyEvent()));
}

ScreenShotMenu::~ScreenShotMenu()
{
    delete ui;
}
