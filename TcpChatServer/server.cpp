#include "server.h"
#include "tcpclientsocket.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::incomingConnection(int socketId)
{
    TcpClientSocket *socket = new TcpClientSocket();
    socket->setSocketDescriptor(socketId);
}

