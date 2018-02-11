#include "tcpchatserver.h"
#include "ui_tcpchatserver.h"

#include "tcpclientsocket.h"
#include "settings.h"

#include <QNetworkConfigurationManager>
#include <QTextCodec>
#include <QDebug>
#include <QDir>

TcpChatServer::TcpChatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TcpChatServer)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/img/server.png"));
    this->setWindowTitle(APPLICATION_RU_NAME);

    server_ = new Server(this);

/* Инициализируем иконку трея, устанавливаем иконку своего приложения,
 * а также задаем всплывающую подсказку
 */
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon(":/img/server.png"));
    trayIcon_->setToolTip("Сервер");
//

// ============= Создаем контекстное меню из двух пунктов =================== //
    QMenu *menu = new QMenu(this);
    QAction *viewWindow = new QAction("Открыть окно Сервер", this);
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

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Получаем сохранённую сетевую конфигурацию
        QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // Если сохранённая сетевая конфигурация в настоящее время не обнаружена, используется системная конфигурация по умолчанию
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession_ = new QNetworkSession(config, this);
        connect(networkSession_, SIGNAL(opened()), this, SLOT(onSessionOpened()));

        ui->statusBar->showMessage("Открытие сессии");
        networkSession_->open();
    } else {
        onSessionOpened();
    }
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
        trayIcon_->showMessage("Сервер",
                               "Приложение свернуто в трей. Для того чтобы, "
                               "развернуть окно приложения, щелкните по иконке приложения в трее",
                               icon, 4000);
    }
}

void TcpChatServer::on_aStartServer_triggered()
{
    if (!server_->listen()) {
        qDebug() << "Сервер не подключен к порту";
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

void TcpChatServer::onSessionOpened()
{
    if (!server_->listen()) {
        QMessageBox::critical(this, tr("Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server_->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // используем первый не локальный адрес IPv4
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // если мы ни одного адреса не нашли, то используем IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->statusBar->showMessage(tr("Сервер запущен, адресс:\n\nIP: %1\nпорт: %2\n\n")
                         .arg(ipAddress).arg(server_->serverPort()));
}


bool TcpChatServer::setRegistrationUser()
{
// ============ Для отображения русских букв в файле Setting.ini ============ //
    QTextCodec *codecForLocaleName = QTextCodec::codecForName("Windows-1251");
    QTextCodec::setCodecForLocale(codecForLocaleName);
//

   QSettings settings(QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/setting.ini"),
                             QSettings::IniFormat);
   settings.setIniCodec(codecForLocaleName);
// Проверка зарегистрирован ли пользователь на сервере
   if (settings.contains("RegistrationLogin/" + Settings::getInstance()->getUserRegistrationLogin().toString())) {
        return false;
   } else {
       settings.beginGroup("RegistrationLogin");
       settings.setValue(Settings::getInstance()->getUserRegistrationLogin().toString(),
                         Settings::getInstance()->getUserRegistrationNickName());
       settings.endGroup();

       settings.beginGroup("RegistrationPassword");
       settings.setValue(Settings::getInstance()->getUserRegistrationLogin().toString(),
                         Settings::getInstance()->getUserRegistrationPassword());
       settings.endGroup();
       return true;
   }


}

bool TcpChatServer::setAuthorizationUser()
{
    QSettings settings(QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/setting.ini"),
                       QSettings::IniFormat);
    if (settings.contains("RegistrationLogin/" + Settings::getInstance()->getUserAuthorizationLogin().toString())) {
        settings.beginGroup("RegistrationPassword");
        QString regPassword = settings.value(Settings::getInstance()->getUserAuthorizationLogin().toString(),
                                             Settings::getInstance()->getUserRegistrationPassword().toString()).toString();
        settings.endGroup();
        if (Settings::getInstance()->getUserAuthorizationPassword() == regPassword) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
