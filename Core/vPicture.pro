#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T22:22:40
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vPicture
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Network/network.cpp \
    Network/Request/request.cpp \
    Parser/parser.cpp \
    Errors/errors.cpp \
    Url/url.cpp \
    Files/files.cpp \
    Upload/upload.cpp \
    DragAndDrop/DragAndDrop.cpp \
    Config/config.cpp \
    Secure/secure.cpp \
    ServerData/serverdata.cpp \
    Network/Cookies/cookiejar.cpp \
    Modules/Screenshot/screenshot.cpp \
    Modules/Screenshot/regionselect.cpp \
    SingleApplication/localserver.cpp \
    SingleApplication/singleapplication.cpp \
    Events/KeyEvent/systemkeyboardreadwrite.cpp \
    Config/settings.cpp \
    Parser/argumentsparser.cpp \
    ../Design/Design/design.cpp \
    Events/Events/events.cpp \
    ../Design/Design/menu.cpp \
    Notification/notification.cpp \
    secondmainwindow.cpp \
    Modules/Screenshot/screenshotmenu.cpp \
    Modules/GifCreator/gifcreator.cpp \
    minioutwidget.cpp \
    Menu/menumain.cpp \
    Modules/OnlineAssistant/onlineassistant.cpp \
    Dialog/dialog.cpp \
    Network/Actions/Connection/connection.cpp \
    Network/Actions/Links/links.cpp \
    Thread/Thread.cpp \
    Thread/worker.cpp \
    Updates/updates.cpp

HEADERS  += mainwindow.h \
    Network/network.h \
    Parser/parser.h \
    Errors/errors.h \
    Url/url.h \
    Files/files.h \
    Upload/upload.h \
    Config/config.h \
    Secure/secure.h \
    ServerData/serverdata.h \
    Network/Cookies/cookiejar.h \
    Modules/Screenshot/screenshot.h \
    Modules/Screenshot/regionselect.h \
    SingleApplication/localserver.h \
    SingleApplication/singleapplication.h \
    Events/KeyEvent/systemkeyboardreadwrite.h \
    Config/settings.h \
    Parser/argumentsparser.h \
    ../Design/Design/menu.h \
    Notification/notification.h \
    secondmainwindow.h \
    Modules/Screenshot/screenshotmenu.h \
    Modules/GifCreator/gifcreator.h \
    minioutwidget.h \
    Menu/menumain.h \
    Modules/OnlineAssistant/onlineassistant.h \
    Dialog/dialog.h \
    Thread/Thread.h \
    Thread/worker.h \
    Updates/updates.h
FORMS    += ../Design/UI/mainwindow.ui \
    ../Design/UI/secondmainwindow.ui \
    ../Design/UI/minioutwidget.ui \
    Modules/Screenshot/screenshotmenu.ui \
    Modules/GifCreator/gifcreator.ui \
    Menu/menumain.ui \
    Modules/OnlineAssistant/onlineassistant.ui \
    Dialog/dialog.ui \
    Updates/updates.ui


QT += network core svg winextras

win32:CONFIG(release, debug|release): LIBS += -LC:/Qt/libqxt-Qt5/lib/
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Qt/libqxt-Qt5/lib/

INCLUDEPATH += C:/Qt/libqxt-Qt5/src/core
DEPENDPATH += C:/Qt/libqxt-Qt5/src/core

INCLUDEPATH += $$PWD/../Debug
DEPENDPATH += $$PWD/../Debug


RESOURCES += \
    ../Design/design.qrc

RC_FILE = icon.rc

TRANSLATIONS = vpicture_fr.ts vpicture_it.ts

mac{
     ICON = icon.icns
}
