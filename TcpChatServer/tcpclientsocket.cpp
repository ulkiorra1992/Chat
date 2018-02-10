#include "tcpclientsocket.h"

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent),
    nextBlockSize(0)
{
    connect(this,SIGNAL(readyRead()), this, SLOT(onReadClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void TcpClientSocket::sendData(const QDate &date, const QTime &time)
{

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << date << time;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}

void TcpClientSocket::onReadClient()
{
    QString login;
    QString userName;
    QString password;

    QDataStream in(this);

    if (nextBlockSize == 0) {
        if (bytesAvailable() < sizeof(quint16))
            return;
        in >> nextBlockSize;
    }

    if (bytesAvailable() < nextBlockSize)
        return;

    quint8 requestType;
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    in >> requestType >> login >> userName >> password;

    if (requestType == 'S') {
        sendData(date, time);
        qDebug() << "avtorization= " << login << password;

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    if (requestType == 'R') {
        sendData(date, time);
        qDebug() << "registration= " << login << userName << password;

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    close();
}

