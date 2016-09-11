#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T17:45:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PushServer
TEMPLATE = app


unix:INCLUDEPATH += /usr/local/include
unix:LIBS += -L/usr/local/lib -lboost_system -lboost_thread

SOURCES += main.cpp\
        mainwindow.cpp \
    pushserver.cpp \
    notifications.cpp \
    remote_client.cpp

HEADERS  += mainwindow.h \
    pushserver.h \
    utility.h \
    notifications.h \
    remote_client.h

FORMS    += mainwindow.ui
