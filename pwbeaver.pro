#-------------------------------------------------
#
# Project created by QtCreator 2019-08-06T10:01:29
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pwbeaver
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++1z

SOURCES += \
        AboutDialog.cpp \
        Charset.cpp \
        MainWindow.cpp \
        PasswordGenerator.cpp \
        Settings.cpp \
        main.cpp

HEADERS += \
        AboutDialog.h \
        Charset.h \
        MainWindow.h \
        PasswordGenerator.h \
        Settings.h

FORMS += \
        aboutdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    license.txt

RESOURCES += \
    pwbeaver.qrc
