#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "tcpchatserver.h"

#include <QTcpSocket>
#include <QDateTime>


class TcpClientSocket : public QTcpSocket
{

    Q_OBJECT

public:
    TcpClientSocket(QObject *parent = 0);

private:
    /**
     * @brief sendData Отправка сервером данных
     * @param date
     * @param time
     */
    void sendData(const QDate &date, const QTime &time);
    quint16 nextBlockSize;

private slots:
    /**
     * @brief onReadClient чтение данных из потока
     */
    void onReadClient();
};

#endif // TCPCLIENTSOCKET_H
