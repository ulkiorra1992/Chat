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
    setWindowIcon(QIcon(":/img/chat.png"));
    this->setWindowTitle("Клиент");

/* Инициализируем иконку трея, устанавливаем иконку своего приложения,
 * а также задаем всплывающую подсказку
 */
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon(":/img/chat.png"));
    trayIcon_->setToolTip("Клиент");
//

// ============= Создаем контекстное меню из двух пунктов =================== //
    QMenu *menu = new QMenu(this);
    QAction *viewWindow = new QAction("Открыть окно Чата", this);
    QAction *quitWindow = new QAction("Выход", this);
//
    connect(viewWindow, SIGNAL(triggered(bool)), this, SLOT(show()));
    connect(quitWindow, SIGNAL(triggered(bool)), this, SLOT(close()));

    menu->addAction(viewWindow);
    menu->addAction(quitWindow);

// Установка контекстного меню на иконку и отображение иконки приложения в трее
    trayIcon_->setContextMenu(menu);
    trayIcon_->show();

// === Подключение сигнала нажатия на иконку к обработчику данного нажатия == //
    connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));

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

void TcpChatClient::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Trigger:
        // Событие игнорируется в том случае, если чекбокс не отмечен
        if (ui->appTray->isChecked()) {
            /* иначе, если окно видимо, то оно скрывается, и наоборот,
             * если скрыто, то разворачивается на экран
             */
            if(!this->isVisible()){
                this->show();
            } else {
                this->hide();
            }
        }
        break;
    default:
        break;
    }
}

void TcpChatClient::closeEvent(QCloseEvent *event)
{
/* Если окно видимо и чекбокс отмечен, то завершение приложения игнорируется, а
 * окно просто скрывается, что сопровождается соответствующим всплывающим сообщением
 */
    if (this->isVisible() && ui->appTray->isChecked()) {
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
                    QSystemTrayIcon::Information);
        trayIcon_->showMessage("Сервер",
                               "Приложение свернуто в трей. Для того чтобы, "
                               "развернуть окно приложения, щелкните по иконке приложения в трее",
                               icon, 4000);
    }
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
    tcpSocket_.connectToHost(QHostAddress("192.168.0.84"), 50050);
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
        quint8 type;
        bool state;
        QStringList list;

        in >> type >> state >> dateToServer >> timeToServer >> list;

        if (type == 'R') {
            if (!state) {
                ui->lblState->setText("<font color=red>Такой логин уже существует !!! </font>");
                ui->lblState->setFont(QFont("Times",12, QFont::Bold));
            } else {
                ui->lblState->setText("");
            }
        }

        if (type == 'A') {
            if (!state) {
                ui->lblState->setText("<font color=red>Не правильный логин или пароль!!! </font>");
                ui->lblState->setFont(QFont("Times",12, QFont::Bold));
            } else {
                ui->lblState->setText("");
            }
        }

        for (int i = 0; i < list.size(); i++) {
            ui->teIn->append(list.at(i));
        }
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
