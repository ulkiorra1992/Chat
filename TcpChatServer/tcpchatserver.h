#ifndef TCPCHATSERVER_H
#define TCPCHATSERVER_H

#include <QMainWindow>
#include <QTcpServer>

#include "server.h"

namespace Ui {
class TcpChatServer;
}

class TcpChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpChatServer(QWidget *parent = 0);
    ~TcpChatServer();

private slots:
    void on_aStartServer_triggered();

private:
    Ui::TcpChatServer *ui;
    Server *server_;
};

#endif // TCPCHATSERVER_H
