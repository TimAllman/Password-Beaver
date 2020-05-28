#-------------------------------------------------
#
# Project created by QtCreator 2019-08-06T10:01:29
#
#-------------------------------------------------

QT += core gui help

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

CONFIG += c++1z strict_c++

# The default is gcc, g++, clang, clang++ depending upon kit.
# All of these are needed in order for qmake to run properly and for the
# build to complete. These show up as corresponding Makefile variables.
# This is done because clang++-10 leads to compile errors in the Qt headers.
QMAKE_CC = clang-9
QMAKE_CXX = clang++-9
QMAKE_LINK = clang++-9


#QMAKE_CXXFLAGS += -Wno-deprecated-copy

## QMAKE_LFLAGS += -static

SOURCES += \
        AboutDialog.cpp \
        CharacterPool.cpp \
        Global.cpp \
        HelpDialog.cpp \
        IndicatorWidget.cpp \
        MainWindow.cpp \
        OptionsManager.cpp \
        PasswordGenerator.cpp \
        main.cpp

HEADERS += \
        AboutDialog.h \
        CharacterPool.h \
        Exceptions.h \
        Global.h \
        HelpDialog.h \
        IndicatorWidget.h \
        MainWindow.h \
        OptionsManager.h \
        PasswordGenerator.h

FORMS += \
        aboutdialog.ui \
        helpdialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    Doxyfile \
    README.md \
    license.txt \
    manual/Manual.html

RESOURCES += \
    pwbeaver.qrc
