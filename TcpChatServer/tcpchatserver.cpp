#include "tcpchatserver.h"
#include "ui_tcpchatserver.h"

#include "tcpclientsocket.h"

#include <QDebug>

TcpChatServer::TcpChatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpChatServer)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/chat.png"));
    this->setWindowTitle(APPLICATION_RU_NAME);

    server_ = new Server();

/* Инициализируем иконку трея, устанавливаем иконку своего приложения,
 * а также задаем всплывающую подсказку
 */
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon(":/img/chat.png"));
    trayIcon_->setToolTip("Чат");
//

// ============= Создаем контекстное меню из двух пунктов =================== //
    QMenu *menu = new QMenu(this);
    QAction *viewWindow = new QAction("Развернуть окно", this);
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
}

TcpChatServer::~TcpChatServer()
{
    delete ui;
}

void TcpChatServer::closeEvent(QCloseEvent *event)
{
/* Если окно видимо и чекбокс отмечен, то завершение приложения игнорируется, а
 * окно просто скрывается, что сопровождается соответствующим всплывающим сообщением
 */
    if (this->isVisible() && ui->cbIsTrayIcon->isChecked()) {
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
                    QSystemTrayIcon::Information);
        trayIcon_->showMessage("Чат",
                               "Приложение свернуто в трей. Для того чтобы, "
                               "развернуть окно приложения, щелкните по иконке приложения в трее",
                               icon, 4000);
    }
}

void TcpChatServer::on_aStartServer_triggered()
{
    if (!server_->listen(QHostAddress::Any, 6178)) {
        qDebug() << "Сервер не слушает порт";
    }
}

void TcpChatServer::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::Trigger:
        // Событие игнорируется в том случае, если чекбокс не отмечен
        if (ui->cbIsTrayIcon->isChecked()) {
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

void TcpChatServer::saveSettings()
{

}
