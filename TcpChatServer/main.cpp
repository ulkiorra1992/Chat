#include "tcpchatserver.h"

#include <QtGui>
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpChatServer server;

    server.show();

    return a.exec();
}
