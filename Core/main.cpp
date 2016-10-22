#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CogDev");
    QCoreApplication::setOrganizationDomain("cogdev.fr");
    QCoreApplication::setApplicationName("vPicture");
    QCoreApplication::setApplicationVersion("Alpha0.11.2.0");

    QApplication app(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name().section("_", 0, 0));
    app.installTranslator(&qtTranslator);

    ArgumentsParser arguments(&app);
    MainWindow w;
    w.setWindowOpacity(0.0);
    QPropertyAnimation animation(&w, "windowOpacity");
    animation.setDuration(1000);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);
    animation.start();
    w.show();
    w.raise();


    return app.exec();
}

