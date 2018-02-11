#include "settings.h"
#include <QDebug>

Settings *Settings::instance_ = 0;

Settings *Settings::getInstance()
{
    if (!instance_) {
        instance_ = new Settings();
    }
    return instance_;
}

void Settings::setUserAuthorizationLogin(QString login)
{
    authorizationLogin_ = login;
}

void Settings::setUserAuthorizationPassword(QString password)
{
    authorizationPassword_ = password;
}

void Settings::setUserAuthorizationIpAdress(QString ip)
{
    ipAdress_ = ip;
}

void Settings::setUserRegistrationLogin(QString login)
{
    registrationLogin_ = login;
}

void Settings::setUserRegistrationNickName(QString nickName)
{
    registrationNickName_ = nickName;
}

void Settings::setUserRegistrationPassword(QString password)
{
    registrationPassword_ = password;
}

void Settings::setUsers(QString login, QString nickName)
{
    users_.insert(login, nickName);
}

void Settings::setAuthorizationUsers(QTcpSocket *soket)
{
    authorizationClients_.append(soket);
}
