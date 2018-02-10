#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QDateTime>


class TcpClientSocket : public QTcpSocket
{

    Q_OBJECT

public:
    TcpClientSocket(QObject *parent = 0);

private:
    void sendData(const QDate &date, const QTime &time);
    quint16 nextBlockSize;

private slots:
    void onReadClient();
};

#endif // TCPCLIENTSOCKET_H
