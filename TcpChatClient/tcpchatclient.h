#ifndef TCPCHATCLIENT_H
#define TCPCHATCLIENT_H

#include "userauthorization.h"

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class TcpChatClient;
}

class TcpChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpChatClient(QWidget *parent = 0);
    ~TcpChatClient();

    void setDataUser(const QString &login, const QString &password);

private:
    Ui::TcpChatClient *ui;
    QTcpSocket tcpSocket_;
    quint16 nextBlockSize_;
    qint8 type_;
    QString login_;
    QString nickName_;
    QString password_;

    void connectionClose();

private slots:
    void on_aUserAuthorization_triggered();
    void onConnectionToServer();
    void onSendRequestToServer();
    void onResponseFromServer();
    void onConnectionClosedByServer();
    void onError();
    void on_tbQuit_clicked();
    void on_aUserRegistration_triggered();
};

#endif // TCPCHATCLIENT_H
