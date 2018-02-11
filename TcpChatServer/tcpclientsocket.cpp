#include "tcpclientsocket.h"
#include "tcpchatserver.h"
#include "settings.h"

#include <QDebug>

TcpClientSocket::TcpClientSocket(QObject *parent) :
    QTcpSocket(parent),
    nextBlockSize(0)
{
    connect(this,SIGNAL(readyRead()), this, SLOT(onReadClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void TcpClientSocket::sendMessage(const QDate &date, const QTime &time)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << date << time;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}

void TcpClientSocket::sendResponse(const quint8 &type, const bool &state, const QDate &date, const QTime &time)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << type << state << date << time;
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

    in >> requestType >> userName >> login >> password ;

    // Авторизация пользователя
    if (requestType == 'A') {

        // проверка данных пользователя при подключении его к серверу
        Settings::getInstance()->setUserAuthorizationLogin(login);
        Settings::getInstance()->setUserAuthorizationPassword(password);

        bool state = TcpChatServer::setAuthorizationUser();
        sendResponse(requestType, state, date, time);

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    // Регистрация нового пользователя
    if (requestType == 'R') {

        // Сохраняем данные пользователя при регистрации его на сервере
        Settings::getInstance()->setUserRegistrationLogin(login);
        Settings::getInstance()->setUserRegistrationNickName(userName);
        Settings::getInstance()->setUserRegistrationPassword(password);

        // если пользователь с таким логином уже зарегистрирован, отправляем ошибку регистрации
        bool state = TcpChatServer::setRegistrationUser();
        sendResponse(requestType, state, date, time);

        QDataStream out(this);
        out << quint16(0xFFFF);
    }

    // Принятие и обработка сообщений
    if (requestType == 'S') {

    }

    close();
}

