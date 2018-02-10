#include "tcpchatserver.h"
#include "ui_tcpchatserver.h"

#include "tcpclientsocket.h"

#include <QDebug>

TcpChatServer::TcpChatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpChatServer)
{
    ui->setupUi(this);
    server_ = new Server();
}

TcpChatServer::~TcpChatServer()
{
    delete ui;
}



void TcpChatServer::on_aStartServer_triggered()
{
    if (!server_->listen(QHostAddress::Any, 6178)) {
        qDebug() << "Server failed to bind to port";
    }
}
