#include "screenshot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenShot w;

    return a.exec();
}
