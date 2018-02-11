#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T11:56:34
#
#-------------------------------------------------

QMAKE_EXTRA_TARGETS += before_build makefilehook

makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild

PRE_TARGETDEPS += .beforebuild

before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251


QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        tcpchatclient.cpp \
    userauthorization.cpp \
    userregistration.cpp

HEADERS  += tcpchatclient.h \
    userauthorization.h \
    userregistration.h \
    projectnamehelper.h

FORMS    += tcpchatclient.ui \
    userauthorization.ui

RESOURCES += \
    res.qrc
