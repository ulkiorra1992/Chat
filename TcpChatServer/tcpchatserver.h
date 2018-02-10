#ifndef TCPCHATSERVER_H
#define TCPCHATSERVER_H

#include <QMainWindow>
#include <QAction>
#include <QSettings>
#include <QTcpServer>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "server.h"

#define APPLICATION_RU_NAME "Сервер"
#define APPLICATION_EN_NAME "Server"

namespace Ui {
class TcpChatServer;
}

class TcpChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpChatServer(QWidget *parent = 0);
    ~TcpChatServer();

    static bool setRegistrationUser();            //!<
    static bool setAuthorizationUser();

protected:
    /**
     * @brief closeEvent Виртуальная функция родительского класса в нашем классе
     * переопределяется для изменения поведения приложения,
     * чтобы оно сворачивалось в трей, когда мы этого хотим
     * @param event событие
     */
    void closeEvent(QCloseEvent *event);

private slots:
    void on_aStartServer_triggered();
    /**
     * @brief onIconActivated обработка принятия сигнала от события нажатия
     * на иконку приложения в трей
     * @param reason
     */
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::TcpChatServer   *ui;
    Server              *server_;
    QSystemTrayIcon     *trayIcon_;         //!< Объявляем объект иконки приложения для трея
};

#endif // TCPCHATSERVER_H
