#-------------------------------------------------
#
# Project created by QtCreator 2019-08-06T10:01:29
#
#-------------------------------------------------

QT += core widgets help

TARGET = pwbeaver
TEMPLATE = app

# The following define makes the compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# These are passed directly to the compiler.
CONFIG += c++1z strict_c++

unix: !macx {
#QMAKE_LFLAGS += -static
#LIBS += -L ~/Qt/5.15.0/Src/qtbase/lib
}

macx {
ICON = icons/icns/beaver.icns
}

win32 {
}

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
    help/help.qhcp \
    help/help.qhp \
    license.txt \
    locale/pwbeaver_fr.qm \
    locale/pwbeaver_fr.ts \
    manual/Manual.html \
    manual/Screenshot.png

RESOURCES += \
    pwbeaver.qrc

TRANSLATIONS = locale/pwbeaver_fr.ts
