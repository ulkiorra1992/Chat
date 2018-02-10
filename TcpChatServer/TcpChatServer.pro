#русская кодировка в выводе приложения
QMAKE_EXTRA_TARGETS += before_build makefilehook

makefilehook.target = $(MAKEFILE)
makefilehook.depends = .beforebuild

PRE_TARGETDEPS += .beforebuild

before_build.target = .beforebuild
before_build.depends = FORCE
before_build.commands = chcp 1251

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpChatServer
TEMPLATE = app


SOURCES += main.cpp\
        tcpchatserver.cpp \
    tcpclientsocket.cpp \
    server.cpp

HEADERS  += tcpchatserver.h \
    tcpclientsocket.h \
    server.h

FORMS    += tcpchatserver.ui

RESOURCES += \
    sources.qrc
