#include "tcpclientsocket.h"
#include "tcpchatserver.h"
#include "settings.h"

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

        // Сохраняем данные пользователя при подключении его к серверу
        Settings::getInstance()->setUserAuthorizationLogin(login);
        Settings::getInstance()->setUserAuthorizationPassword(password);

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    if (requestType == 'R') {
        sendData(date, time);

        // Сохраняем данные пользователя при регистрации его на сервере
        Settings::getInstance()->setUserRegistrationLogin(login);
        Settings::getInstance()->setUserRegistrationNickName(userName);
        Settings::getInstance()->setUserRegistrationPassword(password);

        // если пользователь с таким логином уже зарегистрирован, отправляем ошибку регистрации
        bool state = TcpChatServer::setRegistrationUser();

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    close();
}
