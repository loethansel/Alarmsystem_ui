#-------------------------------------------------
#
# Project created by QtCreator 2019-01-26T16:31:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = alarmsystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    iniparser.cpp \
    ctrlfile.cpp

HEADERS += \
        mainwindow.h \
    iniparser.h \
    ctrlfile.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/debian/Alarmsystem
#$${TARGET}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -licui18n

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -licuuc

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -licudata

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lGLX

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lGLdispatch

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lX11

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lXext

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf


unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lxcb

INCLUDEPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf
DEPENDPATH += $$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf

unix:!macx: LIBS += -L$$PWD/../../../../mnt/beagleroot/usr/lib/arm-linux-gnueabihf/ -lXau -lXdmcp -lbsd
