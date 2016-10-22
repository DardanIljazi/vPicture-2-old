#include "minioutwidget.h"
#include "ui_minioutwidget.h"

MiniOutWidget::MiniOutWidget(QWidget *parent) :
    QWidget(parent, Qt::WindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)),
    ui(new Ui::MiniOutWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton_Logo_Notification, SIGNAL(pressed()), this, SIGNAL(logoPressedEvent()));
    QObject::connect(ui->toolButton_Pictures, SIGNAL(pressed()), this, SIGNAL(picturesEvent()));
    QObject::connect(ui->toolButton_Screenshot, SIGNAL(pressed()), this, SIGNAL(screenshotEvent()));
}

void MiniOutWidget::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasUrls()){
        emit dragEvent(event);
    }
}

MiniOutWidget::~MiniOutWidget()
{
    delete ui;
}
