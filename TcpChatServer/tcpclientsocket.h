#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "tcpchatserver.h"

#include <QTcpSocket>
#include <QDateTime>
#include <QList>


class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    TcpClientSocket(QObject *parent = 0);
    ~TcpClientSocket();

private:
    quint16 nextBlockSize;
    QSet<QTcpSocket*> registrationClients_;
    QSet<QTcpSocket*> authorizationClients_;
    QMap<QTcpSocket*,QString> users_;
    QMap<QString,QString> usersName_;

    void sendMessage(const QDate &date, const QTime &time);
    void sendResponse(const quint8 &type, const bool &state,
                      const QDate &date, const QTime &time);
    void sendResponseAuthorization(QTcpSocket *soket, const quint8 &type, const bool &state,
                                   const QDate &date, const QTime &time,
                                   const QStringList &list);

private slots:
    /**
     * @brief onReadClient чтение данных из потока
     */
    void onReadClient();
};

#endif // TCPCLIENTSOCKET_H
