#ifndef TCPCHATCLIENT_H
#define TCPCHATCLIENT_H

#include "userauthorization.h"

#include <QAction>
#include <QObject>
#include <QTcpSocket>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSystemTrayIcon>


namespace Ui {
class TcpChatClient;
}

class TcpChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpChatClient(QWidget *parent = 0);
    ~TcpChatClient();

    void setDataUser(const QString &login, const QString &password);

private:
    Ui::TcpChatClient *ui;
    QSystemTrayIcon   *trayIcon_;   //!< Объявляем объект иконки приложения для трея

    QTcpSocket tcpSocket_;
    quint16 nextBlockSize_;
    qint8 type_;
    QString login_;
    QString nickName_;
    QString password_;

    void connectionClose();

private slots:
    void on_aUserAuthorization_triggered();
    void onConnectionToServer();
    void onSendRequestToServer();
    void onResponseFromServer();
    void onConnectionClosedByServer();
    void onError();
    void on_tbQuit_clicked();
    void on_aUserRegistration_triggered();
    /**
     * @brief onIconActivated обработка принятия сигнала от события нажатия
     * на иконку приложения в трей
     * @param reason
     */
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    /**
     * @brief closeEvent Виртуальная функция родительского класса в нашем классе
     * переопределяется для изменения поведения приложения,
     * чтобы оно сворачивалось в трей, когда мы этого хотим
     * @param event событие
     */
    void closeEvent(QCloseEvent *event);
};

#endif // TCPCHATCLIENT_H
