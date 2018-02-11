#include "server.h"
#include "tcpclientsocket.h"

#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::incomingConnection(int socketId)
{
    TcpClientSocket *socket = new TcpClientSocket();
    socket->setSocketDescriptor(socketId);

    qDebug() << socket->peerAddress();
}

