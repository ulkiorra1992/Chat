#include "tcpchatclient.h"
#include "ui_tcpchatclient.h"
#include "userregistration.h"

#include <QtNetwork>
#include <QCoreApplication>

TcpChatClient::TcpChatClient(QWidget *parent) :
    QMainWindow(parent),
    nextBlockSize_(0),
    ui(new Ui::TcpChatClient)
{
    ui->setupUi(this);

    connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(onSendRequestToServer()));
    connect(&tcpSocket_, SIGNAL(readyRead()), this, SLOT(onResponseFromServer()));
    connect(&tcpSocket_, SIGNAL(disconnected()),
            this, SLOT(onConnectionClosedByServer()));
    connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError()));
}

TcpChatClient::~TcpChatClient()
{
    delete ui;
}

void TcpChatClient::setDataUser(const QString &login, const QString &password)
{
//    qDebug() << "login=" << login << "password=" << password;
}

void TcpChatClient::connectionClose()
{
    tcpSocket_.close();
}

void TcpChatClient::on_aUserRegistration_triggered()
{
    UserRegistration *registration = new UserRegistration;
    if (registration->exec() == QDialog::Accepted) {
        login_ = registration->userLogin();
        nickName_ = registration->userNickName();
        password_ = registration->userPassword();

        onConnectionToServer();
        type_ = 'R';
    }
    delete registration;
}

void TcpChatClient::on_aUserAuthorization_triggered()
{
    UserAuthorization *autorization = new UserAuthorization;
    if (autorization->exec() == QDialog::Accepted) {
        login_ = autorization->userLogin();
        password_ = autorization->userPassword();

        onConnectionToServer();
        type_ = 'A';
    }
    delete autorization;
}

void TcpChatClient::onConnectionToServer()
{
    tcpSocket_.connectToHost(QHostAddress::LocalHost, 6178);
    ui->statusBar->showMessage("Подключение к серверу...");
    nextBlockSize_ = 0;
}

void TcpChatClient::onSendRequestToServer()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << quint16(0) << quint8(type_) << login_ << nickName_ << password_;

    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    tcpSocket_.write(block);

    ui->statusBar->showMessage("Отправка запроса...");
}

void TcpChatClient::onResponseFromServer()
{
    QDataStream in(&tcpSocket_);
    forever {
        if (nextBlockSize_ == 0) {
            if (tcpSocket_.bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize_;
        }

        if (nextBlockSize_ == 0xFFFF) {
            // когда из потока считаны все данные
            connectionClose();
            ui->statusBar->showMessage("Ответ от сервера получен");
            break;
        }
        if (tcpSocket_.bytesAvailable() < nextBlockSize_) {
            break;
        }

        QDate dateToServer;
        QTime timeToServer;

        in >> dateToServer >> timeToServer ;

        ui->leDataToServer->setText(dateToServer.toString());
        ui->leTimeToServer->setText(timeToServer.toString());
        nextBlockSize_ = 0;
    }
}

void TcpChatClient::onConnectionClosedByServer()
{
    if (nextBlockSize_ != 0xFFFF) {
        ui->statusBar->showMessage("Ошибка: соединение с сервером закрыто");
        connectionClose();
    }
}

void TcpChatClient::onError()
{
    ui->statusBar->showMessage(tcpSocket_.errorString());
    connectionClose();
}


void TcpChatClient::on_tbQuit_clicked()
{
    qApp->exit();
}
