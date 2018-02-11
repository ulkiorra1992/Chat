#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QTcpSocket>
#include <QVariant>
#include <QDir>
#include <QMap>

#include "core_global.h"

class CORESHARED_EXPORT Settings
{
public:
    /**
     * @brief getInstance синглтон.
     * @return возвращает указатель на себя.
     */
    static Settings *getInstance();

    void setUserAuthorizationLogin(QString login);
    void setUserAuthorizationPassword(QString password);
    void setUserAuthorizationIpAdress(QString ip);

    void setUserRegistrationLogin(QString login);
    void setUserRegistrationNickName(QString nickName);
    void setUserRegistrationPassword(QString password);

    void setUsers(QString login, QString nickName);
    void setRemoveUsers(QString login);
    void setAuthorizationUsers(QTcpSocket* soket, QString login);
    void setAuthorizationClient(QTcpSocket* soket);

    QVariant getUserAuthorizationLogin() {return authorizationLogin_; }
    QVariant getUserAuthorizationPassword() { return authorizationPassword_; }
    QVariant getUserAuthorizationIpAdress() {return ipAdress_; }

    QVariant getUserRegistrationLogin() {return registrationLogin_; }
    QVariant getUserRegistrationNickName() {return registrationNickName_; }
    QVariant getUserRegistrationPassword() { return registrationPassword_; }

    QMap<QString, QString> getUsers() {return users_; }
    QMap<QTcpSocket*, QString> getAuthorizationUsers() {return authorizationUsers_; }
    QSet<QTcpSocket*> getAuthorizationClient() {return authorizationClients_; }

private:
    Settings() {

    };
    virtual ~Settings() {

    };

    static Settings *instance_;

    QString authorizationLogin_;
    QString authorizationPassword_;
    QString ipAdress_;

    QString registrationLogin_;
    QString registrationNickName_;
    QString registrationPassword_;

    QMap <QString, QString> users_;
    QMap<QTcpSocket*, QString> authorizationUsers_;
    QSet<QTcpSocket*> authorizationClients_;
};

#endif // SETTINGS_H
