#include "menumain.h"
#include "ui_menumain.h"

MenuMain::MenuMain(QWidget *parent) :
    QWidget(parent, Qt::WindowFlags(Qt::Window | Qt::FramelessWindowHint)),
    ui(new Ui::MenuMain)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    show();       // must be, if not system shadow occur :) I don't know why
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    show();

    setArrowSide();

    this->setGeometry(this->rect());
    this->resize(this->rect().width()+10, this->rect().height()+10);

    QObject::connect(ui->listWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(close()));
}

void MenuMain::addItem(QListWidgetItem *item){
    ui->listWidget->addItem(item);
    this->setGeometry(this->rect());
    this->resize(this->rect().width()+10, this->rect().height()+10);
}

void MenuMain::addItems(QList<QListWidgetItem> *items){
    //    for(int i = 0; i < items->count(); i++)
    //        ui->listWidget->addItem(items->at(i));
}

void MenuMain::popup(int x, int y){
    popup(QPoint(x, y));
}

void MenuMain::popup(QPoint point){
    this->move(point - QPoint(15, 0));
    this->show();
    this->setFocus();
}

void MenuMain::leaveEvent(QEvent *){
    qDebug() << "leaveEvent" << endl;
    if(!this->underMouse()){
        this->close();
    }
}

void MenuMain::closeEvent(QCloseEvent *event){
    static int state = 0;
    if(state == 0){
        this->setWindowOpacity(1.0);
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
        animation->setDuration(150);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->start();

        state ++;
        event->ignore();
        QTimer::singleShot(100, this, SLOT(close()));
    }else{
        //        this->deleteLater();
        state = 0;
        event->accept();
    }
}


void MenuMain::resizeEvent(QResizeEvent *event){
    this->setFixedHeight(8 + ((ui->listWidget->count()) * 21 + (ui->listWidget->count()*6)) + 15);
    ui->listWidget->setFixedHeight(8 + ((ui->listWidget->count()) * 21 + (ui->listWidget->count()*6)));
}

void MenuMain::setArrowSide(int side){
    if(side == MenuMain::Left_Side){
        ui->label_arrow_right->hide();
        ui->label_arrow_left->show();
    }else{
        ui->label_arrow_left->hide();
        ui->label_arrow_right->show();
    }
}

QListWidget* MenuMain::listWidget(){
    return ui->listWidget;
}

MenuMain::~MenuMain()
{
    delete ui;
}
